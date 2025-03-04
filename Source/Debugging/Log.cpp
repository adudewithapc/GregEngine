#include "Log.h"

#include <chrono>
#include <format>
#include <iostream>
#include <Windows.h>

//Naming conflict
#undef ERROR

static greg::log::Level logLevel;
void greg::log::Log(const std::string_view tag, const std::string& message, Level level)
{
    const std::time_t now = std::time(nullptr);
    tm localtime;
    localtime_s(&localtime, &now);

    std::string levelString;
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    switch(level)
    {
        case Level::DEBUG:
            {
                levelString = "Debug";
                break;
            }
        case Level::INFO:
            {
                levelString = "Info";
                break;
            }
        case Level::WARNING:
            {
                levelString = "Warning";
                SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                break;
            }
        case Level::ERROR:
            {
                levelString = "Error";
                SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED);
                break;
            }
        case Level::FATAL:
            {
                levelString = "Fatal";
                SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED);
                break;
            }
    }
    const std::string completeFormat = std::format("[{}:{}:{}][{}][{}] {}", localtime.tm_hour, localtime.tm_min, localtime.tm_sec, levelString, tag, message);
    
    std::cout << completeFormat << std::endl;
    if(level == Level::FATAL)
        exit(-1);
}

void greg::log::Debug(const std::string_view tag, const std::string& message)
{
    Log(tag, message, Level::DEBUG);
}
void greg::log::Info(const std::string_view tag, const std::string& message)
{
    Log(tag, message, Level::INFO);
}
void greg::log::Warning(const std::string_view tag, const std::string& message)
{
    Log(tag, message, Level::WARNING);
}
void greg::log::Error(const std::string_view tag, const std::string& message)
{
    Log(tag, message, Level::ERROR);
}
void greg::log::Fatal(const std::string_view tag, const std::string& message)
{
    Log(tag, message, Level::FATAL);
}

void greg::log::SetLevel(Level level)
{
    logLevel = level;
}
