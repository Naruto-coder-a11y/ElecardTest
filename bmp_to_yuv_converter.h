#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include <bmp_loader.h>

struct YUV420 {
    std::vector<uint8_t> Y;
    std::vector<uint8_t> U;
    std::vector<uint8_t> V;

    int width;
    int height;

    YUV420(int w, int h);
};

void ConvertRGBtoYUV420(const RGBImage &bmp, YUV420& yuv);
void SaveYUV(const std::string &filename, const YUV420& yuv);