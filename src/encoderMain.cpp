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

#include <unordered_map>
#include <cstdlib>
#include <limits>
#include <utility>

// temporary
#include <filesystem>

// inner
#include "DCTCompressor.h"

class Image {
public:
    Image(int width, int height) : width(width), height(height) {
        data.resize(width * height * 3, 0); // 每个像素3个字节(RGB)
    }

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
//std::vector<unsigned char> extractGChannel(const std::vector<unsigned char>& frame_data, size_t width, size_t height) {
std::vector<unsigned char> extractYChannel(const std::vector<unsigned char>& frame_data, size_t width, size_t height) {
//    std::vector<unsigned char> g_channel(width * height);
    std::vector<unsigned char> yComponent(width * height);
    for (int i = 0; i < width * height; ++i) {
        unsigned char r = frame_data[i * 3];
        unsigned char g = frame_data[i * 3 + 1];
        unsigned char b = frame_data[i * 3 + 2];
        // Y = 0.299 * R + 0.587 * G + 0.114 * B (标准公式)
        yComponent[i] = static_cast<unsigned char>(
                0.299 * r + 0.587 * g + 0.114 * b
        );
    }
    return yComponent;

//    for (size_t i = 0; i < width * height; ++i) {
//        g_channel[i] = frame_data[i * 3 + 1];  // 取每个像素的 G 通道（第二个字节）
//    }
//    return g_channel;
}

void ApplyGaussianBlur(const std::vector<unsigned char>& input_frame,
                       std::vector<unsigned char>& output_frame,
                       int width, int height) {
    // 定义高斯核，例如 3x3 的简单核
    std::vector<std::vector<float>> gaussian_kernel = {
            {1 / 16.0f, 2 / 16.0f, 1 / 16.0f},
            {2 / 16.0f, 4 / 16.0f, 2 / 16.0f},
            {1 / 16.0f, 2 / 16.0f, 1 / 16.0f}
    };

    output_frame.resize(input_frame.size());

    // 遍历像素并进行卷积
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            float sum = 0.0f;

            // 卷积计算
            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    int pixel_x = x + kx;
                    int pixel_y = y + ky;
                    int index = pixel_y * width + pixel_x;

                    sum += input_frame[index] * gaussian_kernel[ky + 1][kx + 1];
                }
            }
            output_frame[y * width + x] = static_cast<unsigned char>(std::clamp(sum, 0.0f, 255.0f));
        }
    }
}

// 运动向量结构
struct MotionVector {
    float dx;
    float dy;

    bool IsNearlyZero(float epsilon = 2.5f) const {
        return std::abs(dx) < epsilon && std::abs(dy) < epsilon;
    }
};

// 计算向量模长
float VectorMagnitude(const MotionVector& v) {
    return std::sqrt(v.dx * v.dx + v.dy * v.dy);
}

// 计算向量方向
float VectorDirection(const MotionVector& v) {
    return std::atan2(v.dy, v.dx);
}

// 分块处理
void DivideIntoBlocks(const std::vector<unsigned char>& frame_data,
                      std::vector<std::vector<std::vector<unsigned char>>>& blocks,
                      int width, int height, int block_size) {
    int rows = height / block_size;
    int cols = width / block_size;

    blocks.resize(rows, std::vector<std::vector<unsigned char>>(cols, std::vector<unsigned char>(block_size * block_size)));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            for (int x = 0; x < block_size; ++x) {
                for (int y = 0; y < block_size; ++y) {
                    blocks[i][j][x * block_size + y] = frame_data[(i * block_size + x) * width + (j * block_size + y)];
                }
            }
        }
    }
}

// 计算运动向量
MotionVector ComputeMotionVector(const std::vector<unsigned char>& block,
                                 const std::vector<unsigned char>& previous_frame,
                                 int block_x, int block_y,
                                 int width, int block_size, int search_range) {
    int min_mad = std::numeric_limits<int>::max();
    MotionVector best_vector = {0, 0};

    for (int dx = -search_range; dx <= search_range; ++dx) {
        for (int dy = -search_range; dy <= search_range; ++dy) {
            int mad = 0;
            for (int x = 0; x < block_size; ++x) {
                for (int y = 0; y < block_size; ++y) {
                    int current_x = block_x * block_size + x;
                    int current_y = block_y * block_size + y;
                    int prev_x = current_x + dx;
                    int prev_y = current_y + dy;

                    if (prev_x < 0 || prev_y < 0 || prev_x >= width || prev_y >= width) {
                        mad += 255;
                    } else {
                        mad += std::abs(block[x * block_size + y] -
                                        previous_frame[prev_x * width + prev_y]);
                    }

                    // early-stop
                    if (mad >= min_mad) break;
                }
                if (mad >= min_mad) break;
            }

            if (mad < min_mad) {
                min_mad = mad;
                best_vector = {float(dx), float(dy)};
            }
        }
    }

    return best_vector;
}

void ComputeMotionVectors(const std::vector<std::vector<std::vector<unsigned char>>>& blocks,
                          const std::vector<unsigned char>& previous_frame,
                          std::vector<std::vector<MotionVector>>& motion_vectors,
                          int width, int height, int block_size, int search_range) {
    int rows = height / block_size;
    int cols = width / block_size;
    motion_vectors.resize(rows, std::vector<MotionVector>(cols));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            motion_vectors[i][j] = ComputeMotionVector(blocks[i][j], previous_frame, i, j, width, block_size, search_range);
        }
    }
}

int GetPixelValue(const std::vector<unsigned char>& frame, int x, int y, int width, int height) {
    // 边界镜像处理
    x = std::max(0, std::min(x, width - 1));
    y = std::max(0, std::min(y, height - 1));
    return frame[y * width + x];
}

MotionVector ComputeMotionVector_TSS(const std::vector<unsigned char>& block,
                                     const std::vector<unsigned char>& previous_frame,
                                     int block_x, int block_y,
                                     int width, int height, int block_size, int search_range) {
    int center_dx = 0;
    int center_dy = 0;
    int step_size = std::max(1, search_range / 2); // 初始步长至少为 1
    int min_mad = std::numeric_limits<int>::max();

    // 中心点的坐标
    int block_start_x = block_x * block_size;
    int block_start_y = block_y * block_size;

    MotionVector best_vector = {0, 0};

    while (step_size >= 1) {
        for (int dx = -step_size; dx <= step_size; dx += step_size) {
            for (int dy = -step_size; dy <= step_size; dy += step_size) {
                int mad = 0;

                // 遍历当前块的像素，计算 MAD
                for (int x = 0; x < block_size; ++x) {
                    for (int y = 0; y < block_size; ++y) {
                        int current_x = block_start_x + x;
                        int current_y = block_start_y + y;
                        int prev_x = current_x + center_dx + dx;
                        int prev_y = current_y + center_dy + dy;

                        int current_pixel = block[y * block_size + x];
                        int previous_pixel = GetPixelValue(previous_frame, prev_x, prev_y, width, height);
                        mad += std::abs(current_pixel - previous_pixel);
                    }
                }

                // 更新最优点
                if (mad < min_mad) {
                    min_mad = mad;
                    best_vector = {float(center_dx + dx), float(center_dy + dy)};
                }
            }
        }

        // 更新中心点
        center_dx = static_cast<int>(best_vector.dx);
        center_dy = static_cast<int>(best_vector.dy);

        // 减小步长
        step_size /= 2;
    }

    return best_vector;
}

MotionVector ComputeMotionVector_Optimized(
        const std::vector<unsigned char>& block,
        const std::vector<unsigned char>& previous_frame,
        int block_x, int block_y,
        int width, int height, int block_size, int search_range) {

    int center_dx = 0;
    int center_dy = 0;
    int step_size = std::max(1, search_range / 2); // 初始步长至少为 1
    int min_mad = std::numeric_limits<int>::max();

    // 中心点的坐标
    int block_start_x = block_x * block_size;
    int block_start_y = block_y * block_size;

    MotionVector best_vector = {0, 0};

    // 阶段 1: 使用 TSS 方法快速收敛
    while (step_size >= 1) {
        bool updated = false;
        for (int dx = -step_size; dx <= step_size; dx += step_size) {
            for (int dy = -step_size; dy <= step_size; dy += step_size) {
                int mad = 0;

                // 遍历当前块的像素，计算 MAD
                for (int x = 0; x < block_size; ++x) {
                    for (int y = 0; y < block_size; ++y) {
                        int current_x = block_start_x + x;
                        int current_y = block_start_y + y;
                        int prev_x = current_x + center_dx + dx;
                        int prev_y = current_y + center_dy + dy;

                        int current_pixel = block[y * block_size + x];
                        int previous_pixel = GetPixelValue(previous_frame, prev_x, prev_y, width, height);
                        mad += std::abs(current_pixel - previous_pixel);

                        // 提前终止策略：如果当前 MAD 已经超过最小值，直接跳出循环
                        if (mad >= min_mad) break;
                    }
                    if (mad >= min_mad) break;
                }

                // 更新最优点
                if (mad < min_mad) {
                    min_mad = mad;
                    best_vector = {float(center_dx + dx), float(center_dy + dy)};
                    updated = true;
                }
            }
        }

        // 更新中心点
        if (updated) {
            center_dx = static_cast<int>(best_vector.dx);
            center_dy = static_cast<int>(best_vector.dy);
        }

        // 减小步长
        step_size /= 2;
    }

    // 阶段 2: 在步长为 1 时进行局部全搜索（精细搜索）
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            int mad = 0;

            for (int x = 0; x < block_size; ++x) {
                for (int y = 0; y < block_size; ++y) {
                    int current_x = block_start_x + x;
                    int current_y = block_start_y + y;
                    int prev_x = current_x + center_dx + dx;
                    int prev_y = current_y + center_dy + dy;

                    int current_pixel = block[y * block_size + x];
                    int previous_pixel = GetPixelValue(previous_frame, prev_x, prev_y, width, height);
                    mad += std::abs(current_pixel - previous_pixel);

                    // 提前终止策略：如果当前 MAD 已经超过最小值，直接跳出循环
                    if (mad >= min_mad) break;
                }
                if (mad >= min_mad) break;
            }

            // 更新最优点
            if (mad < min_mad) {
                min_mad = mad;
                best_vector = {float(center_dx + dx), float(center_dy + dy)};
            }
        }
    }

    return best_vector;
}

void ComputeMotionVectors_TSS(const std::vector<unsigned char>& frame,
                              const std::vector<unsigned char>& previous_frame,
                              std::vector<std::vector<MotionVector>>& motion_vectors,
                              int width, int height, int block_size, int search_range) {
    int rows = height / block_size;
    int cols = width / block_size;
    motion_vectors.resize(rows, std::vector<MotionVector>(cols));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // 提取当前块数据
            std::vector<unsigned char> block(block_size * block_size);
            for (int x = 0; x < block_size; ++x) {
                for (int y = 0; y < block_size; ++y) {
                    int idx = (i * block_size + y) * width + (j * block_size + x);
                    block[y * block_size + x] = frame[idx];
                }
            }

            // 调用三步搜索
//            motion_vectors[i][j] = ComputeMotionVector_TSS(block, previous_frame, j, i, width, height, block_size, search_range);
            motion_vectors[i][j] = ComputeMotionVector_Optimized(block, previous_frame, j, i, width, height, block_size, search_range);
        }
    }
}

MotionVector ComputeDominantMotionVector(const std::vector<std::vector<MotionVector>>& motion_vectors) {
    const int bin_count = 36; // 将方向分为 36 个区间（每 10° 一个区间）
    const float bin_size = 2 * M_PI / bin_count; // 每个区间的弧度范围
    const float magnitude_threshold = 1.0f; // 排除过小的运动向量

    std::vector<int> direction_histogram(bin_count, 0);
    std::vector<std::vector<MotionVector>> vector_bins(bin_count);

    int rows = motion_vectors.size();
    int cols = motion_vectors[0].size();

    int small_cnt = 0;

    // 构建方向直方图
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            const MotionVector& mv = motion_vectors[i][j];

            // 计算幅值
            float magnitude = std::sqrt(mv.dx * mv.dx + mv.dy * mv.dy);
            if (magnitude < magnitude_threshold) {
                ++small_cnt;
                continue; // 忽略过小的运动向量
            }

            // 避免过大的异常值影响
            if (magnitude >= 12.f) {
                continue;
            }

            // 计算方向（弧度范围 [0, 2π]）
            float direction = std::atan2(mv.dy, mv.dx);
            if (direction < 0) {
                direction += 2 * M_PI; // 转为正值
            }

            // 找到对应的方向区间
            int bin_index = static_cast<int>(direction / bin_size);
            direction_histogram[bin_index]++;
            vector_bins[bin_index].push_back(mv);
        }
    }

    if (small_cnt > .5f * rows * cols) {
        return {0.f, 0.f};
    }

    // 找到最多向量的方向区间
    int dominant_bin = std::distance(direction_histogram.begin(),
                                     std::max_element(direction_histogram.begin(), direction_histogram.end()));

    // 平均计算该区间内的运动向量
    MotionVector dominant_vector = {0.0f, 0.0f};
    if (!vector_bins[dominant_bin].empty()) {
        for (const MotionVector& mv : vector_bins[dominant_bin]) {
            dominant_vector.dx += mv.dx;
            dominant_vector.dy += mv.dy;
        }
        dominant_vector.dx /= vector_bins[dominant_bin].size();
        dominant_vector.dy /= vector_bins[dominant_bin].size();
    }

    return dominant_vector;
}

/*
// 静止背景检测
void DetectStaticBackground(const std::vector<std::vector<MotionVector>>& motion_vectors,
                            std::vector<std::vector<bool>>& static_mask, float threshold = 5.f) {
    int rows = motion_vectors.size();
    int cols = motion_vectors[0].size();
    static_mask.resize(rows, std::vector<bool>(cols, false));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (VectorMagnitude(motion_vectors[i][j]) < threshold) {
                static_mask[i][j] = true;
            }
        }
    }
}

// 使用RANSAC检测移动背景
void DetectMovingBackground(const std::vector<std::vector<MotionVector>>& motion_vectors,
                            std::vector<std::vector<bool>>& moving_background_mask,
                            int iterations = 300, float inlier_threshold = 0.7f) {
    int rows = motion_vectors.size();
    int cols = motion_vectors[0].size();
    moving_background_mask.resize(rows, std::vector<bool>(cols, false));

    std::vector<std::pair<int, int>> inliers;

    for (int it = 0; it < iterations; ++it) {
        int rand_i = std::rand() % rows;
        int rand_j = std::rand() % cols;

        MotionVector reference_vector = motion_vectors[rand_i][rand_j];
        if (VectorMagnitude(reference_vector) == 0) continue;

        inliers.clear();
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                MotionVector v = motion_vectors[i][j];
                if (std::fabs(VectorDirection(v) - VectorDirection(reference_vector)) < inlier_threshold) {
                    inliers.emplace_back(i, j);
                }
            }
        }

        if (inliers.size() > 0.5 * rows * cols) {
            for (const auto& inlier : inliers) {
                moving_background_mask[inlier.first][inlier.second] = true;
            }
            break;
        }
    }
}

// 前景检测
void DetectForeground(const std::vector<std::vector<bool>>& static_mask,
                      const std::vector<std::vector<bool>>& moving_background_mask,
                      std::vector<std::vector<bool>>& foreground_mask) {
    int rows = static_mask.size();
    int cols = static_mask[0].size();
    foreground_mask.resize(rows, std::vector<bool>(cols, false));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (!static_mask[i][j] && !moving_background_mask[i][j]) {
                foreground_mask[i][j] = true;
            }
        }
    }
}
 */

void DetectForeground(const std::vector<std::vector<MotionVector>>& motion_vectors,
                      std::vector<std::vector<bool>>& foreground_mask,
//                      float direction_threshold = 0.2, // 主导方向允许的最大差值 (弧度)
                      float direction_threshold = 0.3, // 主导方向允许的最大差值 (弧度)
                      float magnitude_threshold = 4, // 主导幅值允许的最大差值
//                      float magnitude_threshold = 0.5, // 主导幅值允许的最大差值
                      int min_connected_region_size = 10) // 最小连通区域大小
{
    int rows = motion_vectors.size();
    int cols = motion_vectors[0].size();

    // Step 1: 初始化掩码
    foreground_mask.assign(rows, std::vector<bool>(cols, false));
    std::vector<std::vector<bool>> background_mask(rows, std::vector<bool>(cols, false));

    // Step 2: 计算主导运动矢量
    MotionVector dominant_vector = ComputeDominantMotionVector(motion_vectors);
    bool bIsStatic = dominant_vector.IsNearlyZero();

    // Step 3: 标记背景区域
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            const MotionVector& mv = motion_vectors[i][j];

            float magnitude_diff = std::abs(VectorMagnitude(mv) - VectorMagnitude(dominant_vector));

            if (bIsStatic) {
                background_mask[i][j] = magnitude_diff <= magnitude_threshold;
                continue;
            }

            // 计算与主导矢量的方向差和幅值差
            float direction_diff = std::abs(VectorDirection(mv) - VectorDirection(dominant_vector));
            if (direction_diff > M_PI) {
                direction_diff = 2 * M_PI - direction_diff; // 归一化到 [0, π]
            }

            // 判断是否为背景
            if (direction_diff <= direction_threshold && magnitude_diff <= magnitude_threshold) {
                background_mask[i][j] = true;
            }
        }
    }

    // Step 4: 标记前景区域
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (!background_mask[i][j]) {
                foreground_mask[i][j] = true;
            }
        }
    }
}

void RefineForeground(const std::vector<std::vector<MotionVector>>& motion_vectors,
                      std::vector<std::vector<bool>>& foreground_mask,
                      int min_region_size) {
    int rows = foreground_mask.size();
    int cols = foreground_mask[0].size();
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::queue<std::pair<int, int>> q;

    auto isInside = [&](int x, int y) {
        return x >= 0 && y >= 0 && x < rows && y < cols;
    };

    float direction_threshold = 0.3f;
    float magnitude_threshold = 4.f;

    auto isSimilar = [&](const MotionVector& v1, const MotionVector& v2) {
        float direction_diff = std::abs(VectorDirection(v1) - VectorDirection(v2));
        if (direction_diff > M_PI) {
            direction_diff = 2 * M_PI - direction_diff;
        }
        float magnitude_diff = std::abs(VectorMagnitude(v1) - VectorMagnitude(v2));
        return direction_diff <= direction_threshold && magnitude_diff <= magnitude_threshold;
    };


    // 遍历每个宏块
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (foreground_mask[i][j] && !visited[i][j]) {
                std::vector<std::pair<int, int>> region;
                q.push({i, j});
                visited[i][j] = true;

                // BFS 收集连通区域
                while (!q.empty()) {
                    auto [x, y] = q.front();
                    q.pop();
                    region.push_back({x, y});

                    // 遍历邻域
                    for (int dx = -1; dx <= 1; ++dx) {
                        for (int dy = -1; dy <= 1; ++dy) {
                            if (std::abs(dx) + std::abs(dy) != 1) continue; // 仅考虑 4 连通
                            int nx = x + dx;
                            int ny = y + dy;

                            if (isInside(nx, ny) && !visited[nx][ny]
                                && isSimilar(motion_vectors[x][y], motion_vectors[nx][ny])) {
                                visited[nx][ny] = true;
                                q.push({nx, ny});
                            }
                        }
                    }
                }

                // 如果区域太小，移除
                if (region.size() < min_region_size) {
                    for (const auto& [x, y] : region) {
                        foreground_mask[x][y] = false;
                    }
                }
            }
        }
    }
}

// 辅助函数：检查是否在边界内
bool InBounds(int x, int y, int width, int height) {
    return x >= 0 && x < width && y >= 0 && y < height;
}

// RemoveSmallRegions函数
void RemoveSmallRegions(std::vector<std::vector<bool>>& foreground_mask, int min_size) {
    int height = foreground_mask.size();
    int width = foreground_mask[0].size();

    // 标记访问状态
    std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));

    // 定义邻居方向（四连通）
    std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    // 遍历整个图像
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // 如果当前像素是前景且未访问过
            if (foreground_mask[y][x] && !visited[y][x]) {
                // 用队列实现BFS进行连通区域分析
                std::queue<std::pair<int, int>> q;
                std::vector<std::pair<int, int>> region_pixels;

                // 初始化BFS
                q.push({x, y});
                visited[y][x] = true;

                while (!q.empty()) {
                    auto [cx, cy] = q.front();
                    q.pop();

                    // 记录当前像素
                    region_pixels.push_back({cx, cy});

                    // 遍历邻居
                    for (const auto& dir : directions) {
                        int nx = cx + dir.first;
                        int ny = cy + dir.second;

                        if (InBounds(nx, ny, width, height) && foreground_mask[ny][nx] && !visited[ny][nx]) {
                            q.push({nx, ny});
                            visited[ny][nx] = true;
                        }
                    }
                }

                // 检查连通区域大小
                if (region_pixels.size() < min_size) {
                    // 如果小于阈值，则移除该区域
                    for (const auto& pixel : region_pixels) {
                        int px = pixel.first;
                        int py = pixel.second;
                        foreground_mask[py][px] = false; // 设置为背景
                    }
                }
            }
        }
    }
}

// 膨胀操作
void Dilate(std::vector<std::vector<bool>>& mask) {
    int rows = mask.size();
    int cols = mask[0].size();
    std::vector<std::vector<bool>> temp_mask = mask;

    std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    for (int i = 1; i < rows - 1; ++i) {
        for (int j = 1; j < cols - 1; ++j) {
            if (mask[i][j]) {
                for (const auto& dir : directions) {
                    int nx = i + dir.first;
                    int ny = j + dir.second;
                    temp_mask[nx][ny] = true;
                }
            }
        }
    }

    mask = temp_mask;
}

void PostProcessing(std::vector<std::vector<bool>>& foreground_mask) {
    // Step 1: 移除较小的前景区域
    RemoveSmallRegions(foreground_mask, 7);
    Dilate(foreground_mask);
}

void Map16x16To8x8(const std::vector<std::vector<bool>>& mask_16x16,
                   std::vector<std::vector<bool>>& mask_8x8,
                   int original_width, int original_height, int block_size_8x8, int block_size_16x16) {
    int rows_16x16 = mask_16x16.size();
    int cols_16x16 = mask_16x16[0].size();

    int rows_8x8 = original_height / block_size_8x8;
    int cols_8x8 = original_width / block_size_8x8;

    mask_8x8.resize(rows_8x8, std::vector<bool>(cols_8x8, false));

    for (int i = 0; i < rows_8x8; ++i) {
        for (int j = 0; j < cols_8x8; ++j) {
            // 找到当前 8x8 块对应的 16x16 块索引
            int block_x = (i * block_size_8x8) / block_size_16x16;
            int block_y = (j * block_size_8x8) / block_size_16x16;

            // 映射值
            if (block_x < rows_16x16 && block_y < cols_16x16) {
                mask_8x8[i][j] = mask_16x16[block_x][block_y];
            }
        }
    }
}

//============

void ComputePixelDifference(const std::vector<unsigned char>& current_frame,
                            const std::vector<unsigned char>& previous_frame,
                            std::vector<std::vector<bool>>& pixel_diff_mask,
                            int width, int height) {
    pixel_diff_mask.resize(height, std::vector<bool>(width, false));

    // 动态阈值 (可根据亮度动态调整)
    int threshold = 15; // 示例固定值，动态可以基于亮度分布计算

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = y * width + x;
            int diff = std::abs(current_frame[index] - previous_frame[index]);

            threshold = current_frame[index] > 200 ? 30
                                            : current_frame[index] > 100 ? 20
                                                                  : 10;

            if (diff > threshold) {
                pixel_diff_mask[y][x] = true;
            }
        }
    }
}

void FuseForegroundMasks(const std::vector<std::vector<bool>>& pixel_diff_mask,
                         const std::vector<std::vector<bool>>& motion_vector_mask,
                         std::vector<std::vector<bool>>& fused_mask,
                         int width, int height) {
    fused_mask.resize(motion_vector_mask.size(), std::vector<bool>(motion_vector_mask[0].size(), false));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // 融合策略：交集为强前景，并集为弱前景
            if (y / 16 >= motion_vector_mask.size() || x / 16 >= motion_vector_mask[0].size()) break;
            if (pixel_diff_mask[y][x] && motion_vector_mask[y / 16][x / 16]) {
                fused_mask[y / 16][x / 16] = true;
            }
        }
    }
}

// 主接口
void IsForeground(const std::vector<unsigned char>& pre_frame_data,
                  const std::vector<unsigned char>& frame_data,
                  bool* identifier, int width, int height) {
    int block_size = 16;
    int search_range = 16;

    std::vector<std::vector<std::vector<unsigned char>>> blocks;
    std::vector<std::vector<MotionVector>> motion_vectors;
    std::vector<std::vector<bool>> static_mask;
    std::vector<std::vector<bool>> moving_background_mask;
    std::vector<std::vector<bool>> foreground_mask;

    std::vector<unsigned char> filtered_frame_data;
    std::vector<unsigned char> filtered_pre_frame_data;
    ApplyGaussianBlur(frame_data, filtered_frame_data, width, height);
    ApplyGaussianBlur(pre_frame_data, filtered_pre_frame_data, width, height);

    // Step 2: 像素差分
    std::vector<std::vector<bool>> pixel_diff_mask;
    ComputePixelDifference(filtered_frame_data, filtered_pre_frame_data, pixel_diff_mask, width, height);

    //=====

    // Step 1: 分块处理
//    DivideIntoBlocks(frame_data, blocks, width, height, block_size);
    DivideIntoBlocks(filtered_frame_data, blocks, width, height, block_size);

    // Step 2: 计算运动向量
//    ComputeMotionVectors(blocks, pre_frame_data, motion_vectors, width, height, block_size, search_range);
    ComputeMotionVectors(blocks, filtered_pre_frame_data, motion_vectors, width, height, block_size, search_range);
//    ComputeMotionVectors_TSS(blocks, pre_frame_data, motion_vectors, width, height, block_size, search_range);
//    ComputeMotionVectors_TSS(frame_data, pre_frame_data, motion_vectors, width, height, block_size, search_range);

    // Step 3: 检测静止背景和移动背景
//    DetectStaticBackground(motion_vectors, static_mask);
//    DetectMovingBackground(motion_vectors, moving_background_mask);

    // Step 4: 检测前景
//    DetectForeground(static_mask, moving_background_mask, foreground_mask);
    DetectForeground(motion_vectors, foreground_mask); // 前景初筛
    RefineForeground(motion_vectors, foreground_mask, 10);

    //=====
    // Step 4: 融合前景检测
    std::vector<std::vector<bool>> foreground_mask_res;
    FuseForegroundMasks(pixel_diff_mask, foreground_mask, foreground_mask_res, width, height);

    // Step 5: 后处理
//    PostProcessing(foreground_mask);
    PostProcessing(foreground_mask_res);

    std::vector<std::vector<bool>> foreground_mask_8x8;
    // Step 5: 将 16x16 掩码映射到 8x8 分辨率
//    Map16x16To8x8(foreground_mask, foreground_mask_8x8, width, height, 8, block_size);
    Map16x16To8x8(foreground_mask_res, foreground_mask_8x8, width, height, 8, block_size);


    // 输出结果到identifier
    int rows = height / 8;
    int cols = width / 8;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            identifier[i * cols + j] = foreground_mask_8x8[i][j];
//            identifier[i * cols + j] = foreground_mask[i][j];
        }
    }
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

    Image image(width, height);
    DCTCompressor dctCompressor;

    std::vector<unsigned char> pre_frame_data(width * height * 3, 0); // 初始时前一帧为空
    std::vector<unsigned char> pre_y_channel(width * height, 0);
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

        // 为节省时间，仅处理前30帧
//        if (frame_id > 400) break;

        const std::vector<unsigned char>& frame_data = image.getData();

        if (isFirstFrame) {
            // 如果是第一帧，直接跳过
            isFirstFrame = false;
            pre_y_channel = extractYChannel(frame_data, width, height);
        } else {
//             std::vector<unsigned char> pre_g_channel = extractYChannel(pre_frame_data, width, height);
//             std::vector<unsigned char> g_channel = extractYChannel(frame_data, width, height);
             std::vector<unsigned char> y_channel = extractYChannel(frame_data, width, height);
             // 计算前景/背景
             IsForeground(pre_y_channel, y_channel, identifier, width, height);
             pre_y_channel = y_channel;
        }

        // 压缩当前帧
        dctCompressor.Compress(frame_data, identifier, foreground_compress_factor, background_compress_factor, outputFile);

        // 将当前帧数据保存为下一帧的前一帧
//        std::memcpy(pre_frame_data.data(), frame_data.data(), frame_data.size());

    }

    delete[] identifier;
    inputStream.close();
}

int main(int argc, char* argv[]) {

    // 假设输入和输出文件
//    const std::string VIDEO_NAME = "WalkingStaticBackground";
//    const std::string VIDEO_NAME = "Village";
//    const std::string VIDEO_NAME = "WalkingMovingBackground";
//    std::string VIDEO_NAME = "SAL";
//    const std::string VIDEO_NAME = "Stairs";
//    const std::string VIDEO_NAME = "Village";
//    const std::string VIDEO_NAME = "orange";
//    const std::string VIDEO_NAME = "car";

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <VIDEO_NAME>" << std::endl;
        return 1;
    }
    std::string VIDEO_NAME = argv[1];

    std::string ROOT_DIRECTORY = "assets/rgbs/";
//    std::string ROOT_DIRECTORY = "../assets/rgbs/";
    std::string OUTPUT_DIRECTORY = "assets/outputs/";
//    std::string OUTPUT_DIRECTORY = "../assets/outputs/";
    std::string inputFile = ROOT_DIRECTORY + VIDEO_NAME + ".rgb";
    std::string outputFile = OUTPUT_DIRECTORY + VIDEO_NAME + ".cmp";

    // 图像的宽度和高度
    int width = 960;
    int height = 540;
//    height = 1696;

    // 压缩因子
    int foreground_compress_factor = 0;
//    int background_compress_factor = 5;
    int background_compress_factor = 0;

    // 处理视频流
    processVideoStream(inputFile, outputFile, width, height, foreground_compress_factor, background_compress_factor);

    return 0;
}
