#pragma once

#include <memory>
#include <string_view>

#include "AudioFile.h"

//A music clip will start playing when the "Play" function is called, repeating until either "Stop" is called or when destroyed
class MusicClip
{
public:
    MusicClip(const std::string_view filePath);

    MusicClip(const MusicClip& other) = delete;
    MusicClip& operator=(const MusicClip& other) = delete;
    
    MusicClip(MusicClip&& other) noexcept = default;
    MusicClip& operator=(MusicClip&& other) noexcept = default;

    ~MusicClip();

    void Play();
    void Stop();

private:
    std::unique_ptr<AudioFile> musicFile;
};
