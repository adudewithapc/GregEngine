#include "ScopedTimer.h"

#include <iostream>

ScopedTimer::ScopedTimer(const std::string_view scopeName)
    : scopeName(scopeName),
      start(std::chrono::high_resolution_clock::now())
{
}

ScopedTimer::~ScopedTimer()
{
    using namespace std::chrono;
    auto end = high_resolution_clock::now();
    high_resolution_clock::duration difference = end - start;
    double time = duration_cast<duration<double>>(difference).count();
    std::cout << "Scope named \"" << scopeName << "\" took " << time << "s" << "\n";
}
