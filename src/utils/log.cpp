#include "log.h"
#include "config.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <Windows.h>
#include <cstdio>
#include <memory>

static std::shared_ptr<spdlog::logger> s_consoleLogger;
static std::shared_ptr<spdlog::logger> s_fileLogger;

void logger::init()
{
    auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("RageOpenRDR2.log", true);
    fileSink->set_pattern("%T.%e|Thread %=5t|%^%=8l%$|%v");

    s_fileLogger = std::make_shared<spdlog::logger>("RageOpenRDR2_logfile", fileSink);
    s_fileLogger->set_level(spdlog::level::trace);
    s_fileLogger->flush_on(spdlog::level::trace);

    if (config::get_config<bool>("console"))
    {
        AllocConsole();

        FILE* unused = nullptr;
        freopen_s(&unused, "CONIN$", "r", stdin);
        freopen_s(&unused, "CONOUT$", "w", stdout);
        freopen_s(&unused, "CONOUT$", "w", stderr);

        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        consoleSink->set_pattern("%T.%e|Thread %=5t|%^%=8l%$|%v");

        s_consoleLogger = std::make_shared<spdlog::logger>("RageOpenRDR2_console", consoleSink);
        s_consoleLogger->set_level(spdlog::level::trace);
    }
}

void logger::vwrite(const char* type, const char* msg, va_list args)
{
    char buffer[256]{ 0 };

    int offset = snprintf(buffer, sizeof(buffer), "[%s]", type);

    if (offset < 0 || offset >= (int)sizeof(buffer))
        offset = sizeof(buffer) - 1;

    vsnprintf(buffer + offset, sizeof(buffer) - offset, msg, args);

    if (s_consoleLogger)
        s_consoleLogger->log(spdlog::level::info, "{}", buffer);

    if (config::get_log(type) && s_fileLogger)
        s_fileLogger->log(spdlog::level::info, "{}", buffer);
}

// This function will log things that happen rarely
void logger::write(const char* type, const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    vwrite(type, msg, args);
    va_end(args);
}

// If targeting ReleaseWithLogs, this function will log inside hooks that are on hot paths
void logger::log(const char* type, const char* msg, ...)
{
#ifdef _LOG
    va_list args;
    va_start(args, msg);
    vwrite(type, msg, args);
    va_end(args);
#endif
}