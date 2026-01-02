/*
    logging.h

    This header provides functions to write structured and readable kernel logs.
*/

#pragma once

#include "klib/types.h"

// different levels of logs
typedef enum {
    LOG_INFO,           // everything is going well
    LOG_WARN,           // something happened but it's not critical
    LOG_ERROR,          // something bad happened, the task must stop
    LOG_PANIC,          // something VERY BAD happened, the kernel must stop
    LOG_DEBUG           // debug info, useful for the developers
} log_level_t;

// basic log function
void log(log_level_t level, string s, ...);
// wrappers for log()
void info(string s, ...);
void warning(string s, ...);
void error(string s, ...);
void panic(string s, ...);
void debug(string s, ...);