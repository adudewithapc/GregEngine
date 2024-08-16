#include "Image.h"

#include <iostream>

#include "Math/Color.h"

void PrintRemaining(std::ifstream& file)
{
    int count = 0;
    while(file)
    {
        char byte = greg::image::Read<char>(file);
        std::cout << std::hex << (int) byte << " ";
        if(++count % 10 == 0)
            std::cout << "\n";
    }
}

unsigned char* greg::image::LoadBMP(const std::string& fileName, int& width, int& height, int& channels)
{
    std::ifstream file(fileName.c_str(), std::ios::in | std::ios::binary);

    if( Read<char>(file) != 'B' ||  Read<char>(file) != 'M')
        return nullptr;

    //File size
    Skip(file, 4);

    //Reserved
    Skip(file, 4);

    unsigned int dataOffset = Read<unsigned int>(file);
    
    unsigned int headerSize = Read<unsigned int>(file);
    if(headerSize != 124)
    {
        std::cerr << "Tried to read a file with a header size of " << headerSize << " when only a size of 124 is currently supported.\n";
        return nullptr;
    }
    
    width = Read<long>(file);
    //Height may be either negative or positive. If positive we need to flip the image vertically
    int uncheckedHeight = Read<long>(file);
    bool flipVertically = uncheckedHeight > 0;
    height = std::abs(uncheckedHeight);

    //Planes (needs to be 1)
    if(Read<short>(file) != 1)
        return nullptr;

    int bitDepth = Read<unsigned short>(file);

    //Compression method. Must be BITFIELDS (3)
    if(Read<int>(file) != 3)
        return nullptr;

    //Image byte size
    Skip(file, 4);
    //Resolution
    Skip(file, 8);
    //Used + important color
    Skip(file, 8);

    unsigned int redMask = Read<unsigned int>(file);
    unsigned int greenMask = Read<unsigned int>(file);
    unsigned int blueMask = Read<unsigned int>(file);
    unsigned int alphaMask = Read<unsigned int>(file);

    channels = 4;

    //Color space
    Skip(file, 40);

    //Skip to color data
    Skip(file, dataOffset - 110);

    unsigned char* colors = new unsigned char[width * height];
    size_t byteCount = width * height * 4;
    for(size_t i = 0; i < byteCount; i++)
    {
        colors[i] = Read<unsigned char>(file);
    }

    if(flipVertically)
    {
        size_t widthByteCount = width * 4;
        size_t heightByteCount = height * 4;
        for(size_t y = 0; y < widthByteCount / 2; y++)
        {
            for(size_t x = 0; x < heightByteCount; x++)
            {
                
            }
        }
    }

    return colors;
}

void greg::image::Skip(std::ifstream& stream, std::streamsize count)
{
    if(!stream)
    {
        std::cerr << "Tried to read file data without any file";
        return;
    }
    stream.ignore(count);
}