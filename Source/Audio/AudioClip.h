#pragma once

#include <memory>
#include <string>
#include "AudioFile.h"

class AudioClip
{
public:
    //Loads and creates a new audio file from the given path. Path must contain file extension.
    AudioClip(const std::string_view filePath);

    AudioClip(const AudioClip&) = delete;
    AudioClip& operator=(const AudioClip&) = delete;

    AudioClip(AudioClip&&) = default;
    AudioClip& operator=(AudioClip&&) = default;

    void Play();
    void Pause();
    void Resume();
    void Stop();
private:
    std::unique_ptr<AudioFile> audioFile;
};
