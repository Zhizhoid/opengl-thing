#pragma once

#include <string>

namespace logs
{
    // TODO: add dump option to logs

    enum class Level
    {
        Debug = 0,
        Info,
        Warning,
        Error,
    };

    void log(Level level, const std::string &formatStr, ...);
}