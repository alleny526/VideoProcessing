## DCTCompressor Class

To use the DCTCompressor class, you need to create its instance and use it.

### Compress

Compress single frame of rgb image using DCT. 

``` cpp
void Compress(const std::vector<unsigned char> &data, const bool *identifier, int foreground_compress_factor, int background_compress_factor)
```

- data: rgb image data
- identifier: list of boolean values to identify foreground and background pixels, true for foreground and false for background.
- foreground_compress_factor: compress factor for foreground pixels (n1 in project description).
- background_compress_factor: compress factor for background pixels (n2 in project description).

This method will compress the image and append the compressed data to file ```compressed_data.txt```.

In order to compress multiple frames, you can call this method multiple times and the compressed data will be appended to the file.

We compress the data to the following format:
```
quantization_factor coeff1 coeff2 … coeff64 coeff1 coeff2 … coeff64 coeff1 coeff2 … coeff64
quantization_factor coeff1 coeff2 … coeff64 coeff1 coeff2 … coeff64 coeff1 coeff2 … coeff64
…
quantization_factor coeff1 coeff2 … coeff64 coeff1 coeff2 … coeff64 coeff1 coeff2 … coeff64
r g b r g b r g b … r g b (The bottom blocks that cannot be divided by 8)
```

### Decompress

Decompress the compressed data from file ```compressed_data.txt```.

``` cpp
void Decompress(std::string file_name);
```

- file_name: the file to store the output rgb data.

The decompressed data will be stored in the same format as the input data, ie, 
```aiignore
result[i] = {r1, g1, b1, r2, g2, b2, …, rN, gN, bN}
```

Finally, you can use the Video player to play the rgb file.

