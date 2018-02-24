//
// Created by chenkuan on 17-12-11.
//

#ifndef IOTEVENTMONITORPLATFORM_LOGGER_H
#define IOTEVENTMONITORPLATFORM_LOGGER_H

#include <string>
#include <ctime>
#include "logConf.h"
using std::string;

enum LoggerLevel {
    debug,
    info,
    warning,
    error
};

class Logger {
  public:
    static Logger *getLogger();
    void setLogLevel(enum LoggerLevel loggerLevel);

    void debug(const char *fmt, ...);
    void info(const char *fmt, ...);
    void warning(const char *fmt, ...);
    void error(const char *fmt, ...);

    void timeBegin();
    void timeEnd();
    void showTime(const string &str);

  private:
    Logger() = default;
    static Logger *logger;
    enum LoggerLevel loggerLevel = LoggerLevel::debug;

#ifdef SHOW_RUN_TIME
    clock_t startTime = 0;
    clock_t endTime = 0;
#endif

    time_t tt = 0;
    static const unsigned TIME_FMT_BUF_MAX_SIZE = 128;
    char timeFmtBuf[TIME_FMT_BUF_MAX_SIZE] = {0};

    void fmtCurTime();
};

#endif //IOTEVENTMONITORPLATFORM_LOGGER_H
