#ifndef FILELOGGER_H
#define FILELOGGER_H

#include "spdlog/spdlog.h"

class FileLogger
{
public:
    FileLogger();

    typedef enum {
        trace = 0,
        debug = 1,
        info = 2,
        warn = 3,
        err = 4,
        critical = 5,
        off = 6
    } level_enum;

    int init(const char* filename, size_t max_file_size, size_t max_files);
    int log(level_enum lvl, const char* msg);

private:
    std::shared_ptr<spdlog::logger> file_logger;
};


extern int initLog(const char* filename,
                   size_t max_file_size,
                   size_t max_files);
extern int logFile(FileLogger::level_enum lvl,
                   const char* msg);

#endif // FILELOGGER_H
