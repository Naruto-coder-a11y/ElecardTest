#pragma once
#ifndef BMP_LOADER_H
#define BMP_LOADER_H

#include <vector>
#include <string>
#include <cstdint>

struct RGBPixel {
    uint8_t r, g, b;
};

struct RGBImage {
    int width;
    int height;
    std::vector<RGBPixel> pixels;
};

// Загрузка BMP 24-битного изображения
RGBImage load_bmp(const std::string& filename);

#endif // BMP_LOADER_H