#ifndef __DATA_CENTER_LOG_H
#define __DATA_CENTER_LOG_H

#define DATA_CENTER_USE_LOG 0

#if DATA_CENTER_USE_LOG

#else
#   define DC_LOG_INFO(...)
#   define DC_LOG_WARN(...)
#   define DC_LOG_ERROR(...)
#endif


#endif
