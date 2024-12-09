## Encoder Template

```cpp
# include "DCTCompressor.h"

int main() {
    int n1, n2;  // read from the command line
    
    int frame_number;  // compute number of frames
    
    unsigned char **data = ReadImageFromAllen("input.rgb");  // data[i] is the i-th frame
    
    bool **identifier = SegmentFromCheng(data);  // read raw data and return the foreground/background identifier
    
    DCTCompressor compressor;

    for (int i = 0; i < frame_number; i++) {
        compressor.Compress(data[i], identifier[i], n1, n2);  // compress the i-th frame with n1 and n2
    }
    
    return 0;
}
```

## Decoder Template

```cpp
# include "DCTCompressor.h"
# include <cstdlib>
# include <iostream>

int main(int argc, char* argv[]) {
    DCTCompressor compressor;
    compressor.Decompress("output.rgb");  // decompressed vedio stored in output.rgb in the format suitable for the player.
    
    // Code to call the player to play output.rgb
    // Construct command to play decoded video
    // argv[1] = sample_file.rgb
    // argv[2] = sample_audio.wav
    std::string play_command = "./MyImageApplication " + argv[1] +
                                " output.rgb" + argv[2] + " 960 540 30";
    
    std::cout << "Decompressing and preparing to play: " << play_command << std::endl;
    
    // Execute the video player with decoded file
    int result = system(play_command.c_str());
    
    if (result != 0) {
        std::cerr << "Failed to play decoded video" << std::endl;
        return 1;
    }
    
    return 0;
}
```

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

