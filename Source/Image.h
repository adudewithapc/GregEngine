﻿#pragma once
//Not properly implemented

#include <fstream>
#include <iostream>

namespace greg
{
    namespace image
    {
        unsigned char* LoadBMP(const std::string& file, int& width, int& height, int& channels);

        template<typename T>
        static T Read(std::ifstream& stream)
        {
            T value;
            stream.read(reinterpret_cast<char*>(&value), sizeof(T));
            return value;
        }

        static void Skip(std::ifstream& stream, std::streamsize count);
    }
}
