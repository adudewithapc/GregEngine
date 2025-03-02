#pragma once

//An interface designed to handle loading, playing etc of a specific audio file type
class AudioFile
{
public:
    virtual void Play() = 0;
    virtual void PlayLooping() = 0;
    virtual void Stop() = 0;
    virtual void Pause() = 0;
    virtual void Resume() = 0;
};
