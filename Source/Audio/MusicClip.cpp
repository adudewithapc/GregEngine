#include "MusicClip.h"

#include <format>
#include "../Debugging/Log.h"
#include "MP3AudioFile.h"

MusicClip::MusicClip(const std::string_view filePath)
{
    //TODO: We currently assume that a file is not already loaded.
    //This can cause issues in cases where a file path is used as a file handle, like for MP3 files.
    if(filePath.ends_with(".mp3"))
    {
        std::wstring wideFilePath;
        wideFilePath.assign(filePath.begin(), filePath.end());
        musicFile = std::make_unique<MP3AudioFile>(wideFilePath);
    }
    else
    {
        greg::log::Error("Audio", std::format("\"{}\" is not a supported audio file extension!", filePath.substr(filePath.find_last_of('.'))));
    }
}
MusicClip::~MusicClip()
{
    Stop();
}
void MusicClip::Play()
{
    musicFile->PlayLooping();
}
void MusicClip::Stop()
{
    musicFile->Stop();
}
