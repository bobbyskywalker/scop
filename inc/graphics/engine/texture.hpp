#pragma once

#include <string>

[[nodiscard]] unsigned char *loadTextureData(int *width, int *height, int *nrChannels, std::string path);

void freeTextureData(unsigned char* data);
