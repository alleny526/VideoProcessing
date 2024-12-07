# include "DCTCompressor.h"

#include <vector>
#include <cmath>

void convertIntToDouble(const int intArray[DCTCompressor::DCTSize][DCTCompressor::DCTSize], double doubleArray[DCTCompressor::DCTSize][DCTCompressor::DCTSize]) {
    for (int i = 0; i < DCTCompressor::DCTSize; ++i) {
        for (int j = 0; j < DCTCompressor::DCTSize; ++j) {
            doubleArray[i][j] = static_cast<double>(intArray[i][j]);
        }
    }
}

void convertDoubleToInt(const double doubleArray[DCTCompressor::DCTSize][DCTCompressor::DCTSize], int intArray[DCTCompressor::DCTSize][DCTCompressor::DCTSize]) {
    for (int i = 0; i < DCTCompressor::DCTSize; ++i) {
        for (int j = 0; j < DCTCompressor::DCTSize; ++j) {
            intArray[i][j] = static_cast<int>(std::round(doubleArray[i][j]));
        }
    }
}

void matrixMultiply(double A[DCTCompressor::DCTSize][DCTCompressor::DCTSize], double B[DCTCompressor::DCTSize][DCTCompressor::DCTSize], double result[DCTCompressor::DCTSize][DCTCompressor::DCTSize]) {
    for (int i = 0; i < DCTCompressor::DCTSize; ++i) {
        for (int j = 0; j < DCTCompressor::DCTSize; ++j) {
            result[i][j] = 0.0;
            for (int k = 0; k < DCTCompressor::DCTSize; ++k) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Constructor
void DCTCompressor() {
    // Compute the DCT matrix
    for (int i = 0; i < DCTCompressor::DCTSize; ++i) {
        for (int j = 0; j < DCTCompressor::DCTSize; ++j) {
            if (i == 0) {
                DCTCompressor::DCTMatrix[i][j] = sqrt(1.0 / DCTCompressor::DCTSize);
            } else {
                DCTCompressor::DCTMatrix[i][j] = sqrt(2.0 / DCTCompressor::DCTSize) * cos(((2 * j + 1) * i * M_PI) / (2 * DCTCompressor::DCTSize));
            }
        }
    }

    // Transpose the DCT matrix
    for (int i = 0; i < DCTCompressor::DCTSize; ++i) {
        for (int j = 0; j < DCTCompressor::DCTSize; ++j) {
            DCTCompressor::DCTMatrix_transpose[i][j] = DCTCompressor::DCTMatrix[j][i];
        }
    }
}

int *DCTCompressor::DCTLocalCompressor(int blocks[DCTSize][DCTSize], int quality) {
    // Create a new int * to store the compressed data
    double input[DCTSize][DCTSize], compressedLocalData[DCTSize][DCTSize], temp[DCTSize][DCTSize];
    int output[DCTSize][DCTSize];
    int result[DCTSize * DCTSize];

    // Convert the input to double
    convertIntToDouble(blocks, input);

    // perform 8*8 DCT on the blocks
    matrixMultiply(DCTMatrix, input, temp);
    matrixMultiply(temp, DCTMatrix_transpose, compressedLocalData);

    // Convert the compressed data to int
    convertDoubleToInt(compressedLocalData, output);

    // Quantize the data
    for (int i = 0; i < DCTSize; ++i) {
        for (int j = 0; j < DCTSize; ++j) {
            output[i][j] = output[i][j] >> quality;
            result[i*DCTSize + j] = output[i][j] << quality;
        }
    }

    return result;
}

int (*DCTCompressor::DCTLocalDecompressor(int *compressedLocalData))[DCTCompressor::DCTSize] {
    double temp[DCTSize][DCTSize], decompressedDoubleData[DCTSize][DCTSize];
    int decompressedLocalData[DCTSize][DCTSize], quantizedData[DCTSize][DCTSize];

    // Read the compressed data
    for (int i = 0; i < DCTSize; ++i) {
        for (int j = 0; j < DCTSize; ++j) {
            quantizedData[i][j] = compressedLocalData[i * DCTSize + j];
        }
    }

    // Convert the quantized data to double
    convertIntToDouble(quantizedData, decompressedDoubleData);

    // Perform inverse DCT
    matrixMultiply(DCTMatrix_transpose, decompressedDoubleData, temp);
    matrixMultiply(temp, DCTMatrix, decompressedDoubleData);

    // Convert the decompressed data to int
    convertDoubleToInt(decompressedDoubleData, decompressedLocalData);

    return decompressedLocalData;
}

int *DCTCompressor::Compress(unsigned char *inData, int width, int height, int foreground_quality, int background_quality, std::pmr::vector<bool> &identifier) {
    // Create a new unsigned char * to store the compressed data
    int size = 10;
    int *compressedData = new int[3 * width * height];

    // Compress the data using DCT
    // ...

    // Save compressedData to a file
    // ...
}

unsigned char *DCTCompressor::Decompress(unsigned char *inData, int width, int height) {
}


