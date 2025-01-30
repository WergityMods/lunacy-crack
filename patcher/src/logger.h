#pragma once

#include <android/log.h>

#define LOGCAT_TAG "LUNACY_CRACK"

enum log_level_t
{
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
};

namespace logger
{
    void print(log_level_t log_level, const char* fmt, ...);
}