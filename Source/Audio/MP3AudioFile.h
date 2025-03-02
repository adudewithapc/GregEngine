#pragma once
#include "AudioFile.h"
#include <fstream>

class MP3AudioFile : public AudioFile
{
public:
    MP3AudioFile(const std::wstring& fileName);

    MP3AudioFile(const MP3AudioFile&) = delete;
    MP3AudioFile& operator=(const MP3AudioFile&) = delete;

    MP3AudioFile(MP3AudioFile&&) = default;
    MP3AudioFile& operator=(MP3AudioFile&&) = default;

    virtual ~MP3AudioFile();
    
    void Play() override;
    void Stop() override;
    void Pause() override;
    void Resume() override;

private:
    void PerformBasicAction(const std::wstring& action);
    
    std::wstring fileName;
};
