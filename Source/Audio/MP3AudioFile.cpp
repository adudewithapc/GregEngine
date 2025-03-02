#include "MP3AudioFile.h"

#include <format>
#include "../Debugging/Log.h"

#include "Windows.h"

MP3AudioFile::MP3AudioFile(const std::wstring& fileName)
    : fileName(fileName)
{
    std::wstring openFormat = L"open \"" + fileName + L"\" type mpegvideo alias mp3";
    MCIERROR error = mciSendString(openFormat.c_str(), nullptr, 0, nullptr);
    if(error)
    {
        std::string narrowFileName;
        narrowFileName.assign(fileName.begin(), fileName.end());
        greg::log::Error("Audio", std::format("Failed to load audio file \"{}\"", narrowFileName));
    }
}
MP3AudioFile::~MP3AudioFile()
{
    PerformBasicAction(L"close");
}

void MP3AudioFile::Play()
{
    PerformBasicAction(L"play");
}

void MP3AudioFile::PlayLooping()
{
    mciSendString((L"play " + fileName + L" repeat").c_str(), nullptr, 0, nullptr);
}

void MP3AudioFile::Pause()
{
    PerformBasicAction(L"pause");
}

void MP3AudioFile::Resume()
{
    PerformBasicAction(L"resume");
}

void MP3AudioFile::Stop()
{
    PerformBasicAction(L"stop");
}

void MP3AudioFile::PerformBasicAction(const std::wstring& action)
{
    std::wstring actionFormat = action + L" \"" + fileName + L"\"";
    if(mciSendString(actionFormat.c_str(), nullptr, 0, nullptr) != 0)
    {
        std::string narrowActionFormat;
        narrowActionFormat.assign(actionFormat.begin(), actionFormat.end());
        greg::log::Error("Audio", std::format("Failed to perform audio action \"{}\"!", narrowActionFormat));
    }
}
