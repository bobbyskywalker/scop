#define STB_IMAGE_IMPLEMENTATION
#include "graphics/engine/texture.hpp"
#include "graphics/image/stb_image.h"

[[nodiscard]] unsigned char *loadTextureData(int *width, int *height, int *nrChannels, std::string path) {
	unsigned char *data = stbi_load(path.c_str(), width, height, nrChannels, 0);
	return data;
}

void freeTextureData(unsigned char* data) {
    if (data) {
        stbi_image_free(data);
    }
}
