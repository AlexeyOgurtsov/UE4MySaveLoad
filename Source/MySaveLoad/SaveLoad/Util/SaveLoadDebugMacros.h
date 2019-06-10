#pragma once

#include "Util/Core/MyDebugMacros.h"

DECLARE_LOG_CATEGORY_EXTERN(MySaveLoadLog, Log, All);

// ~String debug macros Begin
#define SL_DEBUG_LOGFUNC_PREFIX M_DEBUG_LOGFUNC_PREFIX
// ~String debug macros End

// ~Logging macros Begin
#define SL_LOG_CUSTOM_TO(LogCategory, LogLevel, FormatString, ...) M_LOG_CUSTOM_TO(LogCategory, LogLevel, ##__VA_ARGS__);
#define SL_LOG_CUSTOM_TO_IF(ShouldLog, LogCategory, LogLevel, FormatString, ...) M_LOG_CUSTOM_TO_IF(ShouldLog, LogCategory, LogLevel, ##__VA_ARGS__);
#define SL_LOG_CUSTOM_TO_IF_FLAGS(LogFlags, LogCategory, LogLevel, FormatString, ...) M_LOG_CUSTOM_TO_IF_FLAGS(LogFlags, LogCategory, LogLevel, ##__VA_ARGS__);

#define SL_LOG_TO(LogCategory, FormatString, ...)  M_LOG_TO(LogCategory, FormatString, ##__VA_ARGS__);
#define SL_LOG_TO_IF(ShouldLog, LogCategory, FormatString, ...)  M_LOG_TO_IF(ShouldLog, LogCategory, FormatString, ##__VA_ARGS__);
#define SL_LOG_TO_IF_FLAGS(LogFlags, LogCategory, FormatString, ...)  M_LOG_TO_IF_FLAGS(LogFlags, LogCategory, FormatString, ##__VA_ARGS__);

#define SL_LOG_WARN_TO(LogCategory, FormatString, ...) M_LOG_WARN_TO(LogCategory, FormatString, ##__VA_ARGS__);
#define SL_LOG_WARN_TO_IF(ShouldLog, LogCategory, FormatString, ...) M_LOG_WARN_TO_IF(ShouldLog, LogCategory, FormatString, ##__VA_ARGS__);
#define SL_LOG_WARN_TO_IF_FLAGS(LogFlags, LogCategory, FormatString, ...) M_LOG_WARN_TO_IF_FLAGS(LogFlags, LogCategory, FormatString, ##__VA_ARGS__);

#define SL_LOG_ERROR_TO(LogCategory, FormatString, ...) M_LOG_ERROR_TO(LogCategory, FormatString, ##__VA_ARGS__);
#define SL_LOG_ERROR_TO_IF(ShouldLog, LogCategory, FormatString, ...) M_LOG_ERROR_TO_IF(ShouldLog, LogCategory, FormatString, ##__VA_ARGS__);
#define SL_LOG_ERROR_TO_IF_FLAGS(LogFlags, LogCategory, FormatString, ...) M_LOG_ERROR_TO_IF_FLAGS(LogFlags, LogCategory, FormatString, ##__VA_ARGS__);

#define SL_LOG_FATAL_ERROR_TO(LogCategory, FormatString, ...) M_LOG_FATAL_ERROR_TO(LogCategory, FormatString, ##__VA_ARGS__);
#define SL_LOG_FATAL_ERROR_TO_IF(ShouldLog, FormatString, ...) M_LOG_FATAL_ERROR_TO_IF(ShouldLog, LogCategory, FormatString, ##__VA_ARGS__);
#define SL_LOG_FATAL_ERROR_TO_IF_FLAGS(LogFlags, FormatString, ...) M_LOG_FATAL_ERROR_TO_IF_FLAGS(LogFlags, LogCategory, FormatString, ##__VA_ARGS__);

#define SL_EMPTY_TO(LogCategory) M_EMPTY_TO(LogCategory);
#define SL_EMPTY_MSG_TO(LogCategory, FormatString, ...) M_EMPTY_MSG_TO(LogCategory, FormatString, ##__VA_ARGS__);

#define SL_NOT_IMPL_TO(LogCategory, FormatString, ...) M_NOT_IMPL_TO(LogCategory, FormatString, ##__VA_ARGS__);
#define SL_NOT_IMPL_RET_TO(ReturnValue, LogCategory, FormatString, ...) M_NOT_IMPL_RET_TO(ReturnValue, LogCategory, FormatString, ##__VA_ARGS__);

#define SL_TO_BE_IMPL_TO(LogCategory, FormatString, ...) M_TO_BE_IMPL_TO(LogCategory, FormatString, ##__VA_ARGS__);
#define SL_TO_BE_IMPL_RET_TO(ReturnValue, LogCategory, FormatString, ...) M_TO_BE_IMPL_RET_TO(ReturnValue, LogCategory, FormatString, ##__VA_ARGS__);

#define SL_IMPL_NOTE_TO(LogCategory, FormatString, ...) M_IMPL_NOTE_TO(LogCategory, FormatString, ##__VA_ARGS__);

#define SL_LOG(FormatString, ...) SL_LOG_TO(MySaveLoadLog, FormatString, ##__VA_ARGS__);
#define SL_LOG_IF(ShouldLog, FormatString, ...) SL_LOG_TO_IF(ShouldLog, MySaveLoadLog, FormatString, ##__VA_ARGS__);
#define SL_LOG_IF_FLAGS(LogFlags, FormatString, ...) SL_LOG_TO_IF_FLAGS(LogFlags, MySaveLoadLog, FormatString, ##__VA_ARGS__);

#define SL_LOG_WARN(FormatString, ...) SL_LOG_WARN_TO(MySaveLoadLog, FormatString, ##__VA_ARGS__);
#define SL_LOG_WARN_IF(ShouldLog, FormatString, ...) SL_LOG_WARN_TO_IF(ShouldLog, MySaveLoadLog, FormatString, ##__VA_ARGS__);
#define SL_LOG_WARN_IF_FLAGS(LogFlags, FormatString, ...) SL_LOG_WARN_TO_IF_FLAGS(LogFlags, MySaveLoadLog, FormatString, ##__VA_ARGS__);

#define SL_LOG_ERROR(FormatString, ...) SL_LOG_ERROR_TO(MySaveLoadLog, FormatString, ##__VA_ARGS__);
#define SL_LOG_ERROR_IF(ShouldLog, FormatString, ...) SL_LOG_ERROR_TO_IF(ShouldLog, MySaveLoadLog, FormatString, ##__VA_ARGS__);
#define SL_LOG_ERROR_IF_FLAGS(LogFlags, FormatString, ...) SL_LOG_ERROR_TO_IF_FLAGS(LogFlags, MySaveLoadLog, FormatString, ##__VA_ARGS__);

#define SL_LOG_FATAL_ERROR(FormatString, ...) SL_LOG_FATAL_ERROR_TO(MySaveLoadLog, FormatString, ##__VA_ARGS__);
#define SL_LOG_FATAL_ERROR_IF(ShouldLog, FormatString, ...) SL_LOG_FATAL_ERROR_TO_IF(ShouldLog, MySaveLoadLog, FormatString, ##__VA_ARGS__);
#define SL_LOG_FATAL_ERROR_IF_FLAGS(LogFlags, FormatString, ...) SL_LOG_FATAL_ERROR_TO_IF_FLAGS(LogFlags, MySaveLoadLog, FormatString, ##__VA_ARGS__);

#define SL_NOT_IMPL(FormatString, ...) SL_NOT_IMPL_TO(MySaveLoadLog, FormatString, ##__VA_ARGS__);
#define SL_TO_BE_IMPL(FormatString, ...) SL_TO_BE_IMPL_TO(MySaveLoadLog, FormatString, ##__VA_ARGS__);
#define SL_IMPL_NOTE(FormatString, ...) SL_IMPL_NOTE_TO(MySaveLoadLog, FormatString, ##__VA_ARGS__);
#define SL_NOT_IMPL_RET(ReturnValue, FormatString, ...) SL_NOT_IMPL_RET_TO(ReturnValue, MySaveLoadLog, FormatString, ##__VA_ARGS__);
#define SL_TO_BE_IMPL_RET(ReturnValue, FormatString, ...) SL_TO_BE_IMPL_RET_TO(ReturnValue, MySaveLoadLog, FormatString, ##__VA_ARGS__);


#define SL_EMPTY() M_EMPTY();
#define SL_EMPTY_MSG(FormatString, ...) SL_EMPTY_MSG_TO(MySaveLoadLog, FormatString, ##__VA_ARGS__);

// ~Scoped helpres Begin (@warning: NEVER include implementation in brackets, because it will limit the scope)
#define SL_LOGFUNC() M_LOGFUNC_TO(MySaveloadLog);
#define SL_LOGFUNC_MSG(FormatString, ...) M_LOGFUNC_MSG_TO(MySaveLoadLog, FormatString, ##__VA_ARGS__);
// ~Scoped helpres End

#define SL_LOGBLOCK(FormatString, ...) M_LOGBLOCK_TO(MySaveLoadLog, FormatString, ##__VA_ARGS__);