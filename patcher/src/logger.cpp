#include "logger.h"

void logger::print(log_level_t log_level, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    switch (log_level)
    {
        case LOG_LEVEL_INFO:
            __android_log_vprint(ANDROID_LOG_INFO, LOGCAT_TAG, fmt, args);
            break;

        case LOG_LEVEL_DEBUG:
            __android_log_vprint(ANDROID_LOG_DEBUG, LOGCAT_TAG, fmt, args);
            break;

        case LOG_LEVEL_WARNING:
            __android_log_vprint(ANDROID_LOG_WARN, LOGCAT_TAG, fmt, args);
            break;

        case LOG_LEVEL_ERROR:
            __android_log_vprint(ANDROID_LOG_ERROR, LOGCAT_TAG, fmt, args);
            break;
    }

    va_end(args);
}