# include "DCTCompressor.h"

#include <cmath>
#include <fstream>
#include <sstream>

void ConvertIntToDouble(const int intArray[DCTCompressor::DCT_SIZE][DCTCompressor::DCT_SIZE], double doubleArray[DCTCompressor::DCT_SIZE][DCTCompressor::DCT_SIZE]) {
    for (int i = 0; i < DCTCompressor::DCT_SIZE; ++i) {
        for (int j = 0; j < DCTCompressor::DCT_SIZE; ++j) {
            doubleArray[i][j] = static_cast<double>(intArray[i][j]);
        }
    }
}

void ConvertDoubleToInt(const double doubleArray[DCTCompressor::DCT_SIZE][DCTCompressor::DCT_SIZE], int intArray[DCTCompressor::DCT_SIZE][DCTCompressor::DCT_SIZE]) {
    for (int i = 0; i < DCTCompressor::DCT_SIZE; ++i) {
        for (int j = 0; j < DCTCompressor::DCT_SIZE; ++j) {
            intArray[i][j] = static_cast<int>(std::round(doubleArray[i][j]));
        }
    }
}

void MatrixMultiply(double A[DCTCompressor::DCT_SIZE][DCTCompressor::DCT_SIZE], double B[DCTCompressor::DCT_SIZE][DCTCompressor::DCT_SIZE], double result[DCTCompressor::DCT_SIZE][DCTCompressor::DCT_SIZE]) {
    for (int i = 0; i < DCTCompressor::DCT_SIZE; ++i) {
        for (int j = 0; j < DCTCompressor::DCT_SIZE; ++j) {
            result[i][j] = 0.0;
            for (int k = 0; k < DCTCompressor::DCT_SIZE; ++k) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void DCTCompressor() {
    // Compute the DCT matrix
    for (int i = 0; i < DCTCompressor::DCT_SIZE; ++i) {
        for (int j = 0; j < DCTCompressor::DCT_SIZE; ++j) {
            if (i == 0) {
                DCTCompressor::DCT_MATRIX[i][j] = sqrt(1.0 / DCTCompressor::DCT_SIZE);
            } else {
                DCTCompressor::DCT_MATRIX[i][j] = sqrt(2.0 / DCTCompressor::DCT_SIZE) * cos(((2 * j + 1) * i * M_PI) / (2 * DCTCompressor::DCT_SIZE));
            }
        }
    }

    // Transpose the DCT matrix
    for (int i = 0; i < DCTCompressor::DCT_SIZE; ++i) {
        for (int j = 0; j < DCTCompressor::DCT_SIZE; ++j) {
            DCTCompressor::DCT_MATRIX_TRANSPOSE[i][j] = DCTCompressor::DCT_MATRIX[j][i];
        }
    }
}

int *DCTCompressor::DCTLocalCompressor(int blocks[DCT_SIZE][DCT_SIZE], const int quality) {
    // Create a new int * to store the compressed data
    double input[DCT_SIZE][DCT_SIZE], compressed_local_data[DCT_SIZE][DCT_SIZE], temp[DCT_SIZE][DCT_SIZE];
    int output[DCT_SIZE][DCT_SIZE];
    int result[DCT_SIZE * DCT_SIZE];

    // Convert the input to double
    ConvertIntToDouble(blocks, input);

    // perform 8*8 DCT on the blocks
    MatrixMultiply(DCT_MATRIX, input, temp);
    MatrixMultiply(temp, DCT_MATRIX_TRANSPOSE, compressed_local_data);

    // Convert the compressed data to int
    ConvertDoubleToInt(compressed_local_data, output);

    // Quantize the data
    for (int i = 0; i < DCT_SIZE; ++i) {
        for (int j = 0; j < DCT_SIZE; ++j) {
            result[i*DCT_SIZE+j] = output[i][j] >> quality;
        }
    }

    return result;
}

int (*DCTCompressor::DCTLocalDecompressor(const int *compressed_local_data, const int quality))[DCTCompressor::DCT_SIZE] {
    double temp[DCT_SIZE][DCT_SIZE], decompressed_double_data[DCT_SIZE][DCT_SIZE];
    int decompressed_local_data[DCT_SIZE][DCT_SIZE], quantized_data[DCT_SIZE][DCT_SIZE];

    // Read the compressed data
    for (int i = 0; i < DCT_SIZE; ++i) {
        for (int j = 0; j < DCT_SIZE; ++j) {
            quantized_data[i][j] = compressed_local_data[i * DCT_SIZE + j] << quality;
        }
    }

    // Convert the quantized data to double
    ConvertIntToDouble(quantized_data, decompressed_double_data);

    // Perform inverse DCT
    MatrixMultiply(DCT_MATRIX_TRANSPOSE, decompressed_double_data, temp);
    MatrixMultiply(temp, DCT_MATRIX, decompressed_double_data);

    // Convert the decompressed data to int
    ConvertDoubleToInt(decompressed_double_data, decompressed_local_data);

    return decompressed_local_data;
}

// format of the compressed data:
// quantization_factor coeff1 coeff2 … coeff64 coeff1 coeff2 … coeff64 coeff1 coeff2 … coeff64
// quantization_factor coeff1 coeff2 … coeff64 coeff1 coeff2 … coeff64 coeff1 coeff2 … coeff64
// …
// r g b r g b r g b … r g b (The bottom blocks that cannot divided by 8)
void DCTCompressor::Compress(const std::vector<unsigned char> &data, int foreground_quality, int background_quality, const bool *identifier) {
    std::ofstream output_file;
    output_file.open(COMPRESSED_FILE_NAME, std::ios::app);

    int vertical_block_num = HEIGHT / DCT_SIZE;
    int horizontal_block_num = WIDTH / DCT_SIZE;

    // Compress the data and write to the file
    for (int i = 0; i < vertical_block_num; i++) {
        for (int j = 0; j < horizontal_block_num; j++) {
            int quality = identifier[i * horizontal_block_num + j] ? foreground_quality : background_quality;
            output_file << (identifier[i * horizontal_block_num + j] ? foreground_quality : background_quality) << " ";
            int base_idx = i * WIDTH * DCT_SIZE + j * DCT_SIZE;
            for (int k = 0; k < 3; k++) {
                int blocks[DCT_SIZE][DCT_SIZE];
                for (int l = 0; l < DCT_SIZE; l++) {
                    for (int m = 0; m < DCT_SIZE; m++) {
                        blocks[l][m] = static_cast<int>(data[(base_idx + DCT_SIZE * l + m) * 3 + k]);
                    }
                }
                int *compressed_data = DCTLocalCompressor(blocks, quality);

                output_file << quality << " ";
                for (int l = 0; l < DCT_SIZE * DCT_SIZE; l++) {
                    output_file << compressed_data[l] << " ";
                }
            }
            output_file << std::endl;
        }
    }

    // Write the bottom blocks that cannot divided by 8
    for (int i = vertical_block_num * DCT_SIZE * WIDTH * 3; i < HEIGHT * WIDTH * 3; i++) {
        output_file << data[i] << " ";
    }
    output_file << std::endl;
}

// format of rgb file: rgbrgbrgbrgb…
unsigned char **DCTCompressor::Decompress() {
    // Read the compressed data
    std::ifstream input_file;
    input_file.open(COMPRESSED_FILE_NAME);

    // Read each line of the file
    std::string line;
    std::vector<std::vector<int>> compressed_data;
    while (std::getline(input_file, line)) {
        std::istringstream iss(line);
        std::vector<int> data;
        int num;
        while (iss >> num) {
            data.push_back(num);
        }
        compressed_data.push_back(data);
    }

    // Initialize the result
    const int lines_per_frame = WIDTH * HEIGHT / DCT_SIZE / DCT_SIZE + 1;
    const int frame_num = compressed_data.size() / lines_per_frame;
    std::unique_ptr<unsigned char*[]> result = std::make_unique<unsigned char*[]>(frame_num);
    for (int i = 0; i < frame_num; ++i) {
        result[i] = new unsigned char[WIDTH * HEIGHT * 3];
    }

    // Decompress the data
    int horizontal_block_num = WIDTH / DCT_SIZE;
    int vertical_block_num = HEIGHT / DCT_SIZE;
    int blocks[DCT_SIZE * DCT_SIZE];
    for (int i = 0; i < frame_num; i++) {
        for (int j = 0; j < lines_per_frame - 1; j++) {
            int *line = compressed_data[i * lines_per_frame + j].data();
            int quality = line[0];
            int base_idx = j / horizontal_block_num * WIDTH * DCT_SIZE + (j % horizontal_block_num) * DCT_SIZE;
            for (int k = 0; k < 3; k++) {
                for (int l = 0; l < DCT_SIZE * DCT_SIZE; l++) {
                    for (int m = 0; m < DCT_SIZE * DCT_SIZE; m++) {
                        blocks[m] = line[1 + k * DCT_SIZE * DCT_SIZE + m];
                    }
                    int (*decompressed_data)[DCT_SIZE] = DCTLocalDecompressor(blocks, quality);
                    for (int m = 0; m < DCT_SIZE; m++) {
                        for (int n = 0; n < DCT_SIZE; n++) {
                            result[i][(base_idx + m * WIDTH + n) * 3 + k] = static_cast<unsigned char>(decompressed_data[m][n]);
                        }
                    }
                }
            }
        }

        // Read the bottom blocks that cannot divided by 8
        int *line = compressed_data[i * lines_per_frame + lines_per_frame].data();
        int base_idx = vertical_block_num * DCT_SIZE * WIDTH * 3;
        for (int j = 0; j < HEIGHT * WIDTH * 3 - base_idx; j++) {
            result[i][j + base_idx] = static_cast<unsigned char>(line[j]);
        }
    }

    return result.get();
}


