#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "bmp_loader.h"
#include "bmp_to_yuv_converter.h"
#include "overlayYUV420OnFrame.h"

void processVideoWithOverlay(const std::string& yuvFile, const std::string& bmpFile, const std::string& outputFile) {
    // Загрузка BMP
    RGBImage bmpImage = load_bmp(bmpFile);

    // Преобразование в YUV420
    YUV420 overlayYUV(bmpImage.width, bmpImage.height);
    ConvertRGBtoYUV420(bmpImage, overlayYUV);

    // Объединение YUV420 в один буфер
    size_t ySize = overlayYUV.Y.size();
    size_t uSize = overlayYUV.U.size();
    size_t vSize = overlayYUV.V.size();

    std::vector<uint8_t> overlayBuffer(ySize + uSize + vSize);

    std::copy(overlayYUV.Y.begin(), overlayYUV.Y.end(), overlayBuffer.begin());
    std::copy(overlayYUV.U.begin(), overlayYUV.U.end(), overlayBuffer.begin() + ySize);
    std::copy(overlayYUV.V.begin(), overlayYUV.V.end(), overlayBuffer.begin() + ySize + uSize);

    // Открытие входного YUV файла
    std::ifstream input(yuvFile, std::ios::binary);

    int videoWidth = 176;   
    int videoHeight = 144;
    size_t frameSize = videoWidth * videoHeight * 3 / 2;
    std::vector<uint8_t> frameBuffer(frameSize);

    // Открытие выходного YUV файла
    std::ofstream output(outputFile, std::ios::binary);

    // Центрирование наложения
    int xOffset = (videoWidth - bmpImage.width) / 2;
    int yOffset = (videoHeight - bmpImage.height) / 2;

    while (input.read(reinterpret_cast<char*>(frameBuffer.data()), frameSize)) {
        overlayYUV420OnFrame(frameBuffer.data(), overlayBuffer.data(),
            videoWidth, videoHeight,
            bmpImage.width, bmpImage.height,
            xOffset, yOffset);
        output.write(reinterpret_cast<char*>(frameBuffer.data()), frameSize);
    }
}

int main() {
    std::string yuvInputFile = "akiyo_qcif.yuv";
    std::string bmpOverlayFile = "test.bmp";
    std::string yuvOutputFile = "output.yuv";

    processVideoWithOverlay(yuvInputFile, bmpOverlayFile, yuvOutputFile);

    return 0;
}