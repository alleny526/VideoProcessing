//
// Created by jiec on 2024/12/9.
//

// outer
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
#include <queue>

// temporary
#include <filesystem>

// inner
#include "DCTCompressor.h"

class Image {
public:
    Image(int width, int height) : width(width), height(height) {
        data.resize(width * height * 3, 0); // 每个像素3个字节(RGB)
    }

    // unsigned char* getData() {
    //     return data.data();
    // }

    // const unsigned char* getData() const {
    //     return data.data();
    // }

    const std::vector<unsigned char>& getData() const {
        return data;
    }

    int getWidth() const {
        return width;
    }

    int getHeight() const {
        return height;
    }

    bool readFrame(std::ifstream& videoStream) {
        size_t frameSize = width * height * 3;
        return videoStream.read(reinterpret_cast<char*>(data.data()), frameSize).good();
    }

    bool writeFrame(std::ofstream& videoStream) const {
        size_t frameSize = width * height * 3;
        videoStream.write(reinterpret_cast<const char*>(data.data()), frameSize);
        return videoStream.good();
    }

private:
    int width, height;
    std::vector<unsigned char> data; // 存储RGB数据
};

// 提取 G 通道的数据
std::vector<unsigned char> extractGChannel(const std::vector<unsigned char>& frame_data, size_t width, size_t height) {
    std::vector<unsigned char> g_channel(width * height);
    for (size_t i = 0; i < width * height; ++i) {
        g_channel[i] = frame_data[i * 3 + 1];  // 取每个像素的 G 通道（第二个字节）
    }
    return g_channel;
}

// 判断前景还是背景的函数
void IsForeground(const std::vector<unsigned char>& pre_frame_data, const std::vector<unsigned char>& frame_data, bool* identifier, int width, int height) {
    int block_size = 16;
    int num_blocks_x = width / block_size;
    int num_blocks_y = height / block_size;
    const int motion_threshold = 4;  // Motion vector threshold for background
    const int region_threshold = 10; // Threshold for motion vector similarity to group foreground blocks

    // Step 1: 计算每个宏块的运动矢量
    std::vector<std::pair<int, int>> motion_vectors(num_blocks_x * num_blocks_y);  // Stores motion vector (dx, dy)

    for (int by = 0; by < num_blocks_y; ++by) {
        for (int bx = 0; bx < num_blocks_x; ++bx) {
            int block_index = by * num_blocks_x + bx;

            // 计算运动矢量（使用简单的块匹配）
            int mv_x = 0, mv_y = 0;
            int min_cost = INT_MAX;
            for (int dy = -block_size; dy <= block_size; ++dy) {
                for (int dx = -block_size; dx <= block_size; ++dx) {
                    int cost = 0;
                    for (int j = 0; j < block_size; ++j) {
                        for (int i = 0; i < block_size; ++i) {
                            int pre_pixel_x = bx * block_size + i + dx;
                            int pre_pixel_y = by * block_size + j + dy;
                            int pixel_x = bx * block_size + i;
                            int pixel_y = by * block_size + j;
                            if (pre_pixel_x >= 0 && pre_pixel_x < width && pre_pixel_y >= 0 && pre_pixel_y < height) {
                                int pre_pixel = pre_frame_data[pre_pixel_y * width + pre_pixel_x];
                                int pixel = frame_data[pixel_y * width + pixel_x];
                                cost += std::abs(pre_pixel - pixel);
                            }
                        }
                    }

                    if (cost < min_cost) {
                        min_cost = cost;
                        mv_x = dx;
                        mv_y = dy;
                    }
                }
            }

            motion_vectors[block_index] = {mv_x, mv_y};
        }
    }

    // Step 2: 判断每个宏块是背景还是前景
    for (int by = 0; by < num_blocks_y; ++by) {
        for (int bx = 0; bx < num_blocks_x; ++bx) {
            int block_index = by * num_blocks_x + bx;
            int mv_x = motion_vectors[block_index].first;
            int mv_y = motion_vectors[block_index].second;

            // 判断运动矢量是否足够小，认为是背景
            if (std::abs(mv_x) < motion_threshold && std::abs(mv_y) < motion_threshold) {
                identifier[block_index] = false;  // Background - static
            } else {
                identifier[block_index] = true;   // Foreground
            }
        }
    }

    // // Step 3: 使用连通性分析进一步优化前景分割
    // std::vector<bool> visited(num_blocks_x * num_blocks_y, false);
    //
    // // 辅助函数：BFS查找连通区域
    // auto bfs = [&](int start_idx) {
    //     std::queue<int> q;
    //     q.push(start_idx);
    //     visited[start_idx] = true;
    //
    //     int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};  // 上下左右四个方向
    //
    //     while (!q.empty()) {
    //         int current = q.front();
    //         q.pop();
    //         int current_by = current / num_blocks_x;
    //         int current_bx = current % num_blocks_x;
    //         int current_mv_x = motion_vectors[current].first;
    //         int current_mv_y = motion_vectors[current].second;
    //
    //         // 遍历相邻的块
    //         for (auto& dir : directions) {
    //             int adj_by = current_by + dir[0];
    //             int adj_bx = current_bx + dir[1];
    //
    //             if (adj_by >= 0 && adj_by < num_blocks_y && adj_bx >= 0 && adj_bx < num_blocks_x) {
    //                 int adj_idx = adj_by * num_blocks_x + adj_bx;
    //                 if (!visited[adj_idx] && identifier[adj_idx] &&
    //                     std::abs(motion_vectors[adj_idx].first - current_mv_x) < region_threshold &&
    //                     std::abs(motion_vectors[adj_idx].second - current_mv_y) < region_threshold) {
    //                     visited[adj_idx] = true;
    //                     q.push(adj_idx);
    //                 }
    //             }
    //         }
    //     }
    // };
    //
    // // 使用 BFS 来找到连通的前景块
    // for (int i = 0; i < num_blocks_x * num_blocks_y; ++i) {
    //     if (identifier[i] && !visited[i]) {
    //         bfs(i);  // 从当前块开始查找连通区域
    //     }
    // }
}


// 从视频流逐帧读取、处理、压缩和写入
void processVideoStream(const std::string& inputFile, const std::string& outputFile, int width, int height, int foreground_compress_factor, int background_compress_factor) {
    std::ifstream inputStream(inputFile, std::ios::binary);
    std::filesystem::path current_path = std::filesystem::current_path();
    std::cout << "Current path: " << current_path << std::endl;
    if (!inputStream) {
        std::cerr << "Failed to open input file: " << inputFile << std::endl;
        return;
    }

    // std::ofstream outputStream(outputFile, std::ios::binary);
    // if (!outputStream) {
    //     std::cerr << "Failed to open output file: " << outputFile << std::endl;
    //     return;
    // }

    Image image(width, height);
    DCTCompressor dctCompressor;

    std::vector<unsigned char> pre_frame_data(width * height * 3, 0); // 初始时前一帧为空
    bool* identifier = new bool[width * height]; // 用于存储前景背景标识
    memset(identifier, false, sizeof(bool) * width * height);

    bool isFirstFrame = true;

    // 逐帧读取并处理
    int frame_id = 0;
    while (inputStream) {
        if (!image.readFrame(inputStream)) {
            break; // 文件读取完成，退出循环
        }
        std::cout << frame_id++ << std::endl;

        // 为节省时间，仅处理前50帧
        if (frame_id > 50) break;

        const std::vector<unsigned char>& frame_data = image.getData();

        if (isFirstFrame) {
            // 如果是第一帧，直接跳过
            isFirstFrame = false;
        } else {
            // std::vector<unsigned char> pre_g_channel = extractGChannel(pre_frame_data, width, height);
            // std::vector<unsigned char> g_channel = extractGChannel(frame_data, width, height);
            // // 计算前景/背景
            // IsForeground(pre_g_channel, g_channel, identifier, width, height);
        }

        // 压缩当前帧
        dctCompressor.Compress(frame_data, identifier, foreground_compress_factor, background_compress_factor, outputFile);

        // 将当前帧数据保存为下一帧的前一帧
        std::memcpy(pre_frame_data.data(), frame_data.data(), frame_data.size());

        // // 将当前帧数据写入输出流
        // image.writeFrame(outputStream);
    }

    delete[] identifier;
    inputStream.close();
    // outputStream.close();
}

int main() {
    // 假设输入和输出文件
    std::string ROOT_DIRECTORY = "../assets/rgbs/";
    // std::string ROOT_DIRECTORY = "../assets/";
    std::string NAME = ROOT_DIRECTORY + "WalkingStaticBackground";
    std::string inputFile = NAME + ".rgb";
    std::string outputFile = NAME + ".cmp";

    // 图像的宽度和高度
    int width = 960;
    int height = 540;

    // 压缩因子
    int foreground_compress_factor = 1;
    int background_compress_factor = 4;

    // 处理视频流
    processVideoStream(inputFile, outputFile, width, height, foreground_compress_factor, background_compress_factor);

    return 0;
}
