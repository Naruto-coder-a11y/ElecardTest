#pragma once
#include <fstream>
#include <stdexcept>

void overlayYUV420OnFrame(uint8_t* frameYUV, const uint8_t* imageYUV, int width, int height, int imageWidth, int imageHeight, int xOffset, int yOffset);
void writeYUVToFile(const uint8_t* yuvBuffer, int width, int height, const char* filename);