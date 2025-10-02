#pragma once

#include <string>

namespace logs
{
    // TODO: add tags to logs
    // TODO: add timestamp to logs
    // TODO: add dump option to logs

    enum class Level
    {
        Debug = 0,
        Info,
        Warning,
        Error,
    };

    void log(Level level, const char *formatStr, ...) [[gnu::format(printf, 2, 3)]];
}