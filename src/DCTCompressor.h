
#ifndef DCTCOMPRESSOR_H
#define DCTCOMPRESSOR_H
#include <vector>

class DCTCompressor {
public:
    static const int DCTSize = 8;
    static double DCTMatrix[DCTSize][DCTSize];
    static double DCTMatrix_transpose[DCTSize][DCTSize];
    /**
     * Compresses the input data using DCT.
     * @param inData The input data to compress.
     * @param width The width of the image.
     * @param height The height of the image.
     * @param foreground_quality The foreground quality of the compression.
     * @param background_quality The background quality of the compression.
     * @param identifier The identifier of foreground or background. 0 for background, 1 for foreground.
     * @return The compressed data.
     */
    static int *Compress(unsigned char *inData, int width, int height, int foreground_quality, int background_quality, std::pmr::vector<bool> &identifier);

    /**
     * Decompresses the input data using DCT.
     * @param inData The input data to decompress.
     * @param width The width of the image.
     * @param height The height of the image.
     * @return The decompressed data.
     */
    static unsigned char *Decompress(unsigned char *inData, int width, int height);

private:
    DCTCompressor();

    static int *DCTLocalCompressor(int blocks[DCTSize][DCTSize], int quality);
    static int (*DCTLocalDecompressor (int *blocks))[DCTSize];
};

#endif //DCTCOMPRESSOR_H
