
#ifndef DCTCOMPRESSOR_H
#define DCTCOMPRESSOR_H
#include <vector>

class DCTCompressor {
public:
    static constexpr int WIDTH = 960;
    static constexpr int HEIGHT = 540;
    static constexpr int DCT_SIZE = 8;
    static double DCT_MATRIX[DCT_SIZE][DCT_SIZE];
    static double DCT_MATRIX_TRANSPOSE[DCT_SIZE][DCT_SIZE];
    static std::string COMPRESSED_FILE_NAME;

    /**
     * Compresses the input data using DCT.
     * @param inData The input data to compress.
     * @param foreground_quality The foreground quality of the compression.
     * @param background_quality The background quality of the compression.
     * @param identifier The identifier of foreground or background. 0 for background, 1 for foreground.
     * generate the compressed data file.
     */
    static void Compress(const std::vector<unsigned char> &data, int foreground_quality, int background_quality, const bool *identifier);

    /**
     * Decompresses the input data using DCT.
     * @return The decompressed rgb files.
     */
    static unsigned char **Decompress();

private:
    DCTCompressor();

    static int *DCTLocalCompressor(int blocks[DCT_SIZE][DCT_SIZE], int quality);

    static int (*DCTLocalDecompressor (const int *compressed_local_data, int quality))[DCT_SIZE];

};

#endif //DCTCOMPRESSOR_H
