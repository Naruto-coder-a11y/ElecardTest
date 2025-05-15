#include "bmp_loader.h"
#include <fstream>
#include <stdexcept>
#include <iostream>

#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
};

struct BMPInfoHeader {
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};
#pragma pack(pop)

RGBImage load_bmp(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);

    BMPFileHeader fileHeader;
    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));

    BMPInfoHeader infoHeader;
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    int width = infoHeader.biWidth;
    int height = infoHeader.biHeight;

    int rowSize = ((width * 3 + 3) / 4) * 4; // Выравнивание до 4 байт
    std::vector<RGBPixel> pixels(width * height);

    file.seekg(fileHeader.bfOffBits, std::ios::beg);

    for (int y = 0; y < height; ++y) {
        int row = height - 1 - y;
        for (int x = 0; x < width; ++x) {
            uint8_t bgr[3];
            pixels[row * width + x] = { bgr[2], bgr[1], bgr[0] }; // Преобразование BGR в RGB
        }

        int padding = rowSize - width * 3;
        if (padding > 0) {
            file.ignore(padding);
        }
    }

    return { width, height, std::move(pixels) };
}