#pragma once

#ifdef DEBUG
#define LOG_LEVEL_INFO
#define LOG_TO_FILE
#define LOG_TO_STDOUT
#else
#define LOG_LEVEL_ERROR
#define LOG_TO_STDOUT
#endif
