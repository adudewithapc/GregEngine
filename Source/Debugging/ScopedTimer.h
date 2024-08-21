#pragma once

#include <chrono>

class ScopedTimer
{
public:
    ScopedTimer(const std::string_view scopeName);
    ~ScopedTimer();
private:
    std::string_view scopeName;
    std::chrono::high_resolution_clock::time_point start;
};
