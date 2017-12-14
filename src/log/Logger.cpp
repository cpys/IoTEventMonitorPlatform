//
// Created by chenkuan on 17-12-11.
//

#include <iostream>
#include <cstdarg>
#include "Logger.h"
using std::clog;
using std::endl;
using std::cerr;

Logger *Logger::logger = nullptr;

Logger *Logger::getLogger() {
    if (Logger::logger == nullptr) {
        Logger::logger = new Logger();
    }
    return Logger::logger;
}

void Logger::setLogLevel(enum LoggerLevel loggerLevel) {
    this->loggerLevel = loggerLevel;
}

void Logger::debug(const char *fmt, ...) {
    if (loggerLevel > LoggerLevel::debug) {
        return;
    }
    fmtCurTime();

    printf("%s [DEBUG] ", timeFmtBuf);

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    printf("\n");
}

void Logger::info(const char *fmt, ...) {
    if (loggerLevel > LoggerLevel::info) {
        return;
    }
    fmtCurTime();

    printf("%s [INFO.] ", timeFmtBuf);

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    printf("\n");
}

void Logger::warning(const char *fmt, ...) {
    if (loggerLevel > LoggerLevel::warning) {
        return;
    }
    fmtCurTime();

    printf("%s [WARN.] ", timeFmtBuf);

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    printf("\n");
}

void Logger::error(const char *fmt, ...) {
    if (loggerLevel > LoggerLevel::error) {
        return;
    }
    fmtCurTime();

    printf("%s [ERROR] ", timeFmtBuf);

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    printf("\n");
}


void Logger::timeBegin() {
#ifdef SHOW_RUN_TIME
    startTime = clock();
#endif
}

void Logger::timeEnd() {
#ifdef SHOW_RUN_TIME
    endTime = clock();
#endif
}

void Logger::showTime(const string &str) {
#ifdef SHOW_RUN_TIME
    fmtCurTime();
    clog << timeFmtBuf << str << (double)(endTime - startTime) / CLOCKS_PER_SEC * 1000 << "ms" << endl;
#endif
}

void Logger::fmtCurTime() {
    // 获取当前时间
    time(&tt);
    // 格式化当前时间
    strftime(timeFmtBuf, TIME_FMT_BUF_MAX_SIZE, "%F %T", localtime(&tt));
}
