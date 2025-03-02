#include "AudioClip.h"

#include <format>

#include "MP3AudioFile.h"
#include "../Debugging/Log.h"

AudioClip::AudioClip(const std::string_view filePath)
{
    //TODO: We currently assume that a file is not already loaded.
    //This can cause issues in cases where a file path is used as a file handle, like for MP3 files.
    if(filePath.ends_with(".mp3"))
    {
        std::wstring wideFilePath;
        wideFilePath.assign(filePath.begin(), filePath.end());
        audioFile = std::make_unique<MP3AudioFile>(wideFilePath);
    }
    else
    {
        greg::log::Error("Audio", std::format("\"{}\" is not a supported audio file extension!", filePath.substr(filePath.find_last_of('.'))));
    }
}

void AudioClip::Play()
{
    audioFile->Play();
}

void AudioClip::Pause()
{
    audioFile->Pause();
}

void AudioClip::Stop()
{
    audioFile->Stop();
}

void AudioClip::Resume()
{
    audioFile->Stop();
}