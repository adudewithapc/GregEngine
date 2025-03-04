#pragma once
#include <string>

//Naming conflict
#undef ERROR

namespace greg
{
namespace log
{

enum class Level : uint8_t
{
    DEBUG = 0,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

void Log(const std::string_view tag, const std::string& message, Level level = Level::INFO);

void Debug(const std::string_view tag, const std::string& message);
void Info(const std::string_view tag, const std::string& message);
void Warning(const std::string_view tag, const std::string& message);
void Error(const std::string_view tag, const std::string& message);
void Fatal(const std::string_view tag, const std::string& message);

void SetLevel(Level level);
}
};
