#include "FileLogger.h"

static FileLogger s_fileLogger;

FileLogger::FileLogger()
{
    file_logger = nullptr;
}

int FileLogger::init(const char *filename,
                      size_t max_file_size,
                      size_t max_files)
{
    try {
        file_logger = spdlog::rotating_logger_mt(
                    "file_logger", filename,
                    max_file_size, max_files);
    }
    catch (const spdlog::spdlog_ex& ex) {
        return -1;
    }

    return 0;
}

int FileLogger::log(FileLogger::level_enum lvl, const char *msg)
{
    try {
        file_logger->log((spdlog::level::level_enum)lvl, msg);
        file_logger->flush();
    }
    catch (const spdlog::spdlog_ex& ex) {
        return -1;
    }

    return 0;
}

int initLog(const char *filename, size_t max_file_size, size_t max_files)
{
    return s_fileLogger.init(filename, max_file_size, max_files);
}

int logFile(FileLogger::level_enum lvl, const char *msg)
{
    return s_fileLogger.log(lvl, msg);
}
