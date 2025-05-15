#include <vector>
#include <cstdint>
#include <string>
#include <bmp_to_yuv_converter.h>
#include <bmp_loader.h>
#include <fstream>

YUV420::YUV420(int w, int h) : width(w), height(h) {
    Y.resize(w * h);
    U.resize(((w + 1) / 2) * ((h + 1) / 2));
    V.resize(((w + 1) / 2) * ((h + 1) / 2));
}

void ConvertRGBtoYUV420(const RGBImage& bmp, YUV420& yuv) {
    int width = bmp.width;
    int height = bmp.height;

    // Проходим по всем пикселям BMP
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Получаем RGB пиксель
            RGBPixel pixel = bmp.pixels[y * width + x];

            // Преобразуем RGB в YUV
            uint8_t Y = static_cast<uint8_t>(0.299 * pixel.r + 0.587 * pixel.g + 0.114 * pixel.b);
            uint8_t U = static_cast<uint8_t>(-0.169 * pixel.r - 0.331 * pixel.g + 0.5 * pixel.b + 128);
            uint8_t V = static_cast<uint8_t>(0.5 * pixel.r - 0.419 * pixel.g - 0.081 * pixel.b + 128);

            // Заполняем буфер Y
            yuv.Y[y * width + x] = Y;

            // Заполняем буферы U и V только для блоков 2x2 пикселей
            if (x % 2 == 0 && y % 2 == 0) {
                int uIndex = (y / 2) * (width / 2) + (x / 2);
                yuv.U[uIndex] = U;
                yuv.V[uIndex] = V;
            }
        }
    }
}

void SaveYUV(const std::string& filename, const YUV420& yuv) {
    std::ofstream out(filename, std::ios::binary);

    out.write(reinterpret_cast<const char*>(yuv.Y.data()), yuv.Y.size());
    out.write(reinterpret_cast<const char*>(yuv.U.data()), yuv.U.size());
    out.write(reinterpret_cast<const char*>(yuv.V.data()), yuv.V.size());
}