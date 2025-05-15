#include <fstream>
#include <stdexcept>
#include <overlayYUV420OnFrame.h>

// Наложение BMP на YUV
void overlayYUV420OnFrame(uint8_t* frameYUV, const uint8_t* imageYUV,

    int width, int height, int imageWidth, int imageHeight,
    int xOffset, int yOffset) {
    int frameYSize = width * height;
    int frameUSize = (width / 2) * (height / 2);
    int uvOffset = frameYSize;

    // Y
    for (int y = 0; y < imageHeight; ++y) {
        for (int x = 0; x < imageWidth; ++x) {
            int frameYIndex = (yOffset + y) * width + (xOffset + x);
            int imageYIndex = y * imageWidth + x;

            if (frameYIndex < frameYSize && imageYIndex < imageWidth * imageHeight) {
                frameYUV[frameYIndex] = imageYUV[imageYIndex];
            }
        }
    }

    // U и V
    for (int y = 0; y < imageHeight / 2; ++y) {
        for (int x = 0; x < imageWidth / 2; ++x) {
            int frameUIndex = uvOffset + (yOffset / 2 + y) * (width / 2) + (xOffset / 2 + x);
            int frameVIndex = frameUIndex + frameUSize;
            int imageUVIndex = (y * (imageWidth / 2)) + x;
            int imageUVBase = imageWidth * imageHeight;

            if (frameUIndex < frameUSize + uvOffset &&
                frameVIndex < frameYSize * 3 / 2 &&
                imageUVIndex < imageWidth * imageHeight / 4) {
                frameYUV[frameUIndex] = imageYUV[imageUVIndex + imageWidth * imageHeight];
                frameYUV[frameVIndex] = imageYUV[imageUVIndex + imageWidth * imageHeight + (imageWidth * imageHeight / 4)];
            }
        }
    }
}

void writeYUVToFile(const uint8_t* yuvBuffer, int width, int height, const char* filename) {
    std::ofstream outFile(filename, std::ios::binary);
    outFile.write(reinterpret_cast<const char*>(yuvBuffer), width * height * 3 / 2);  // Y + UV
    outFile.close();
};