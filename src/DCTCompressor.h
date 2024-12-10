#ifndef DCTCOMPRESSOR_H
#define DCTCOMPRESSOR_H
#include <vector>
#include <string>

class DCTCompressor {
public:
    static constexpr int WIDTH = 960;
    static constexpr int HEIGHT = 540;
    static constexpr int DCT_SIZE = 8;
    // static const std::string COMPRESSED_FILE_NAME;
    double dct_matrix[DCT_SIZE][DCT_SIZE];
    double dct_matrix_transpose[DCT_SIZE][DCT_SIZE];

    DCTCompressor();

    /**
     * Compresses **ONLY ONE FRAME** using DCT. So you may need to call this function multiple times to compress all the frames.
     * @param data The input data to compress.
     * @param foreground_compress_factor The foreground compress_factor of the compression.
     * @param background_compress_factor The background compress_factor of the compression.
     * @param identifier The identifier of foreground or background. 0 for background, 1 for foreground.
     * generate the compressed data file.
     */
    void Compress(const std::vector<unsigned char> &data, const bool *identifier
        , int foreground_compress_factor
        , int background_compress_factor
        , std::string COMPRESSED_FILE_NAME);

    /**
     * Decompresses the input data using DCT.
     * @param result The decompressed rgb files to memory. Initialize the vector as empty.
     * @return The decompressed rgb files to memory.
     */
    void Decompress(std::vector<std::vector<unsigned char>> &result, std::string COMPRESSED_FILE_NAME);

private:
    // Compress a single 8*8 block of data.
    void DCTLocalCompressor(int blocks[DCT_SIZE][DCT_SIZE], int compress_factor, int result[DCT_SIZE * DCT_SIZE]);

    // Decompress a single 8*8 block of DCT data.
    void DCTLocalDecompressor (const int *compressed_local_data, int compress_factor, int decompressed_local_data[DCT_SIZE][DCT_SIZE]);

};

#endif //DCTCOMPRESSOR_H