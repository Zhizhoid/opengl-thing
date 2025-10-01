#include "log.hpp"
#include <cstdio>
#include <cstdarg>

using namespace std::string_literals;

namespace logs
{
    // static function forward declarations

    static constexpr const char *levelToStr(Level level) {
        switch (level) {
        case Level::Debug:
            return "DEBUG";
        case Level::Info:
            return "INFO";
        case Level::Warning:
            return "WARNING";
        case Level::Error:
            return "ERROR";
        default:
            return "UNKNOWN_LEVEL";
        }
    }

    // public functions

    void log(Level level, const std::string &formatStr, ...) {
        std::string formatFinal = levelToStr(level) + ": "s + formatStr + "\n";

        va_list args;
        va_start(args, formatStr);
        std::vprintf(formatFinal.c_str(), args);
        va_end(args);
    }
}