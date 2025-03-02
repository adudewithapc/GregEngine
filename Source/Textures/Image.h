#pragma once
#include <string_view>

struct Image
{
public:
    Image(const std::string& filePath);

    ~Image();
    
    int GetWidth() const;
    int GetHeight() const;
    int GetChannels() const;
    const unsigned char* GetData() const; 

private:
    int width = 0;
    int height = 0;
    int channels = 0;
    unsigned char* data;
};
