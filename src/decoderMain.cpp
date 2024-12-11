//
// Created by jiec on 2024/12/9.
//

// outer
#include <vector>
#include <iostream>
#include <fstream>

// inner
#include "DCTCompressor.h"

void decoderMain(const std::vector<std::vector<unsigned char>>& result, const std::string& output_rgb_file) {
    // 打开输出文件（.rgb 文件）
    std::ofstream output_file(output_rgb_file, std::ios::binary); // 使用binary模式打开

    if (!output_file.is_open()) {
        std::cerr << "Error opening output RGB file!" << std::endl;
        return;
    }

    // 遍历 result 中的所有像素数据
    // 假设 result 是一个二维 vector，每行存储了图像的一行像素，且每个像素的 RGB 通道数据连续存储
    for (const auto& row : result) {
        // 直接按行写入RGB数据（每个元素是一个像素的R, G, B通道）
        output_file.write(reinterpret_cast<const char*>(row.data()), row.size());
    }

    // 关闭输出文件
    output_file.close();

    std::cout << "Decoded RGB data written to: " << output_rgb_file << std::endl;
}

int main(int argc, char* argv[]) {
    // 假设输入和输出文件
//    const std::string VIDEO_NAME = "WalkingStaticBackground";
//    const std::string VIDEO_NAME = "Village";
//    const std::string VIDEO_NAME = "WalkingMovingBackground";
//    const std::string VIDEO_NAME = "SAL";
//    const std::string VIDEO_NAME = "Stairs";
//    const std::string VIDEO_NAME = "Village";
//    const std::string VIDEO_NAME = "orange";
//    const std::string VIDEO_NAME = "car";
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <VIDEO_NAME>" << std::endl;
        return 1;
    }
    std::string VIDEO_NAME = argv[1];

//    std::string ROOT_DIRECTORY = "../assets/outputs/";
    std::string ROOT_DIRECTORY = "assets/outputs/";
    std::string inputFile = ROOT_DIRECTORY + VIDEO_NAME + ".cmp";
    std::string outputFile = ROOT_DIRECTORY + "PROCESSED_" + VIDEO_NAME + ".rgb";

    // 图像的宽度和高度
    int width = 960;
    int height = 540;

    std::cout << "Input file:  starts decompressing" << inputFile << std::endl;
    DCTCompressor dctCompressor;
    std::vector<std::vector<unsigned char>> result;
    dctCompressor.Decompress(result, inputFile);

    std::cout << "Decompressed RGB data written to: " << outputFile << std::endl;
    decoderMain(result, outputFile);


    return 0;
}
