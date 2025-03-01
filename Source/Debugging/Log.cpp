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
        case DEBUG:
            {
                levelString = "Debug";
                break;
            }
        case INFO:
            {
                levelString = "Info";
                break;
            }
        case WARNING:
            {
                levelString = "Warning";
                SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                break;
            }
        case ERROR:
            {
                levelString = "Error";
                SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED);
                break;
            }
        case FATAL:
            {
                levelString = "Fatal";
                SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED);
                break;
            }
    }
    const std::string completeFormat = std::format("[{}:{}:{}][{}][{}] {}", localtime.tm_hour, localtime.tm_min, localtime.tm_sec, levelString, tag, message);
    
    std::cout << completeFormat << std::endl;
    if(level == FATAL)
        exit(-1);
}

void greg::log::Debug(const std::string_view tag, const std::string& message)
{
    Log(tag, message, DEBUG);
}
void greg::log::Info(const std::string_view tag, const std::string& message)
{
    Log(tag, message, INFO);
}
void greg::log::Warning(const std::string_view tag, const std::string& message)
{
    Log(tag, message, WARNING);
}
void greg::log::Error(const std::string_view tag, const std::string& message)
{
    Log(tag, message, ERROR);
}
void greg::log::Fatal(const std::string_view tag, const std::string& message)
{
    Log(tag, message, FATAL);
}

void greg::log::SetLevel(Level level)
{
    logLevel = level;
}
