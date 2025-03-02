#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <ostream>

Image::Image(const std::string& filePath)
    : data(stbi_load(filePath.c_str(), &width, &height, &channels, 0))
{
}

Image::~Image()
{
    stbi_image_free(data);
}

int Image::GetWidth() const
{
    return width;
}
int Image::GetHeight() const
{
    return height;
}
int Image::GetChannels() const
{
    return channels;
}

const unsigned char* Image::GetData() const
{
    return data;
}
