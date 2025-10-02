#include "log.hpp"
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <format>

using namespace std::string_literals;

namespace logs
{
    // static function forward declarations

    static constexpr const char *levelToStr(Level level)
    {
        switch (level)
        {
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

    /**
     * @brief turns "...\\n..." into "...\\n>>...>| ..." (for each '\\n')
     *
     * @param s
     * @param offset offset after each line break !including the space!
     */
    void padNewLines(std::string &s, const size_t offset)
    {
        if (s.find('\n') == std::string::npos)
            return;

        size_t beg = 0;

        std::string padding = [offset]() -> std::string
        {
            if (offset == 0)
                return "";
            if (offset == 1)
                return "|";

            std::string ret(offset, '>');
            ret.replace(offset-2, 2, "| ");
            return ret;
        }();

        // FIXME: a lot of copying...
        while (true)
        {
            beg = s.find('\n', beg);
            if (beg == std::string::npos)
                return;

            s.insert(beg + 1, padding);

            beg += 1 + offset;
        }
    }

    // public functions

    void log(Level level, const char *formatStr, ...)
    {
        // FIXME: really not optimized...

        va_list args;
        va_start(args, formatStr);

        std::string prefix = levelToStr(level) + ": "s;
        std::string fmtFinal = prefix + formatStr;

        // + 1 for terminator
        size_t bufSize = vsnprintf(nullptr, 0, fmtFinal.c_str(), args) + 1;
        char *buf = new char[bufSize];

        // should be fine not using snprintf since buf size is calculated by vsnprintf
        vsprintf(buf, fmtFinal.c_str(), args);

        std::string formatted(buf);

        delete[] buf;

        padNewLines(formatted, prefix.length());

        printf("%s\n", formatted.c_str());

        va_end(args);
    }
}