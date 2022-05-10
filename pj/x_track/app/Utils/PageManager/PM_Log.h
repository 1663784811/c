#ifndef __PM_LOG_H
#define __PM_LOG_H

#define PAGE_MANAGER_USE_LOG 0

#if PAGE_MANAGER_USE_LOG
#   PM_LOG_INFO(format, ...)    printf("\r\nPM INFO: "),printf(format, ##__VA_ARGS__)
#   PM_LOG_WARN(format, ...)    printf("\r\nPM WARN: "),printf(format, ##__VA_ARGS__)
#   PM_LOG_ERROR(format, ...)   printf("\r\nPM ERROR: "),printf(format, ##__VA_ARGS__)
#else
#   define PM_LOG_INFO(...)
#   define PM_LOG_WARN(...)
#   define PM_LOG_ERROR(...)
#endif

#endif
