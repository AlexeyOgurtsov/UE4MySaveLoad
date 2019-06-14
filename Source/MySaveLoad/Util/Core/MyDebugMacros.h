#pragma once

#include "Logging/LogMacros.h"
#include "Log/MyLoggingTypes.h"

/**
* General log: Use this category when you do NOT know what category to use;
*/
DECLARE_LOG_CATEGORY_EXTERN(MyLog, Log, All);

// ~String debug macros Begin
#define M_DEBUG_LOGFUNC_PREFIX (FString(__FUNCTION__) + FString(TEXT(":")) + FString::FromInt(__LINE__))
// ~String debug macros End

// ~Logging macros Begin
#define M_LOG_CUSTOM_TO(LogCategory, LogLevel, FormatString, ...)\
{\
	UE_LOG(LogCategory, LogLevel, TEXT("%s: "),  *M_DEBUG_LOGFUNC_PREFIX, *FString::Printf(FormatString, ##__VA_ARGS__));\
}

#define M_LOG_CUSTOM_TO_IF(ShouldLog, LogCategory, LogLevel, FormatString, ...)\
{\
	if(ShouldLog)\
	{\
		M_LOG_CUSTOM_TO(LogCategory, LogLevel, FormatString, ##__VA_ARGS__);\
	}\
}

#define M_LOG_CUSTOM_TO_IF_FLAGS(LogFlags, LogCategory, LogLevel, FormatString, ...)\
{\
	if(UMyLoggingTypes::ShouldLogVerbosity(LogFlags, ELogVerbosity::Type::LogLevel))\
	{\
		M_LOG_CUSTOM_TO(LogCategory, LogLevel, FormatString, ##__VA_ARGS__);\
	}\
}

#define M_LOG_TO(LogCategory, FormatString, ...)\
{\
	M_LOG_CUSTOM_TO(LogCategory, Log, FormatString, ##__VA_ARGS__);\
}

#define M_LOG_TO_IF(ShouldLog, LogCategory, FormatString, ...)\
{\
	M_LOG_CUSTOM_TO_IF(ShouldLog, LogCategory, Log, FormatString, ##__VA_ARGS__);\
}

#define M_LOG_TO_IF_FLAGS(LogFlags, LogCategory, FormatString, ...)\
{\
	M_LOG_CUSTOM_TO_IF_FLAGS(LogFlags, LogCategory, Log, FormatString, ##__VA_ARGS__);\
}

#define M_LOG_WARN_TO(LogCategory, FormatString, ...)\
{\
	M_LOG_CUSTOM_TO(LogCategory, Warning, FormatString, ##__VA_ARGS__);\
}

#define M_LOG_WARN_TO_IF(ShouldLog, LogCategory, FormatString, ...)\
{\
	M_LOG_CUSTOM_TO_IF(ShouldLog, LogCategory, Warning, FormatString, ##__VA_ARGS__);\
}

#define M_LOG_WARN_TO_IF_FLAGS(LogFlags, LogCategory, FormatString, ...)\
{\
	M_LOG_CUSTOM_TO_IF_FLAGS(LogFlags, LogCategory, Warning, FormatString, ##__VA_ARGS__);\
}

#define M_LOG_ERROR_TO(LogCategory, FormatString, ...)\
{\
	M_LOG_CUSTOM_TO(LogCategory, Error, FormatString, ##__VA_ARGS__);\
}

#define M_LOG_ERROR_TO_IF(ShouldLog, LogCategory, FormatString, ...)\
{\
	M_LOG_CUSTOM_TO_IF(ShouldLog, LogCategory, Error, FormatString, ##__VA_ARGS__);\
}

#define M_LOG_ERROR_TO_IF_FLAGS(LogFlags, LogCategory, FormatString, ...)\
{\
	M_LOG_CUSTOM_TO_IF_FLAGS(LogFlags, LogCategory, Error, FormatString, ##__VA_ARGS__);\
}

#define M_LOG_FATAL_ERROR_TO(LogCategory, FormatString, ...)\
{\
	M_LOG_CUSTOM_TO(LogCategory, Fatal, FormatString, ##__VA_ARGS__);\
}

#define M_LOG_FATAL_ERROR_TO_IF(ShouldLog, LogCategory, FormatString, ...)\
{\
	M_LOG_CUSTOM_TO_IF(ShouldLog, LogCategory, Fatal, FormatString, ##__VA_ARGS__);\
}

#define M_LOG_FATAL_ERROR_TO_IF_FLAGS(LogFlags, LogCategory, FormatString, ...)\
{\
	M_LOG_CUSTOM_TO_IF_FLAGS(LogFlags, LogCategory, Fatal, FormatString, ##__VA_ARGS__);\
}

/**
* Marking body as empty
*/
#define M_EMPTY_TO(LogCategory) {}

/**
* Marking body as empty
*/
#define M_EMPTY_MSG_TO(LogCategory, FormatString, ...) {}

/**
* Fatal non-implemented marking macro.
*/
#define M_NOT_IMPL_TO(LogCategory, FormatString, ...)\
{\
	M_LOG_CUSTOM_TO(LogCategory, Error, TEXT("NOT YET IMPL: %s"), *FString::Printf(FormatString, ##__VA_ARGS__));\
	unimplemented();\
}

#define M_NOT_IMPL_RET_TO(ReturnValue, LogCategory, FormatString, ...)\
{\
	M_NOT_IMPL_TO(LogCategory, FormatString, ##__VA_ARGS__);\
	return ReturnValue; \
}

/**
* Non fatal non-implementing macro.
*/
#define M_TO_BE_IMPL_TO(LogCategory, FormatString, ...)\
{\
	M_LOG_CUSTOM_TO(LogCategory, Warning, TEXT("TO BE IMPL: %s"), *FString::Printf(FormatString, ##__VA_ARGS__));\
}

#define M_TO_BE_IMPL_RET_TO(ReturnValue, LogCategory, FormatString, ...)\
{\
	M_TO_BE_IMPL_TO(LogCategory, FormatString, ##__VA_ARGS__);\
	return ReturnValue; \
}

/**
* Message of implemetation note.
*/
#define M_IMPL_NOTE_TO(LogCategory, FormatString, ...)\
{\
	M_LOG_CUSTOM_TO(LogCategory, Warning, TEXT("TO BE IMPL: %s"), *FString::Printf(FormatString, ##__VA_ARGS__));\
}

/**
* Verifies the return value with CheckExpression and returns the value.
*/
#define M_RETURN_VERIFY(ReturnValue, CheckExpression)\
{\
	verify(CheckExpression);\
	return ReturnValue;\
}

/**
* Verifies the return value with CheckExpression and returns the value.
*/
#define M_RETURN_VERIFY_MSG(ReturnValue, CheckExpression, FormatString, ...)\
{\
	verifyf(CheckExpression, FormatString, ##__VA_ARGS__);\
	return ReturnValue;\
}

/**
* Checks the return value with CheckExpression and returns the value.
*/
#define M_RETURN_CHECK(ReturnValue, CheckExpression)\
{\
	check(CheckExpression);\
	return ReturnValue;\
}

/**
* Checks the return value with CheckExpression and returns the value.
*/
#define M_RETURN_CHECK_MSG(ReturnValue, CheckExpression, FormatString, ...)\
{\
	checkf(CheckExpression, FormatString, ##__VA_ARGS__);\
	return ReturnValue;\
}


#define M_LOG(FormatString, ...)\
{\
	M_LOG_TO(MyLog, FormatString, ##__VA_ARGS__);\
}

#define M_LOG_IF(ShouldLog, FormatString, ...)\
{\
	M_LOG_TO_IF(ShouldLog, MyLog, FormatString, ##__VA_ARGS__);\
}

#define M_LOG_IF_FLAGS(LogFlags, FormatString, ...)\
{\
	M_LOG_TO_IF_FLAGS(LogFlags, MyLog, FormatString, ##__VA_ARGS__);\
}

#define M_LOG_WARN(FormatString, ...)\
{\
	M_LOG_WARN_TO(MyLog, FormatString, ##__VA_ARGS__);\
}
#define M_LOG_WARN_IF(ShouldLog, FormatString, ...)\
{\
	M_LOG_WARN_TO_IF(ShouldLog, MyLog, FormatString, ##__VA_ARGS__);\
}
#define M_LOG_WARN_IF_FLAGS(LogFlags, FormatString, ...)\
{\
	M_LOG_WARN_TO_IF_FLAGS(LogFlags, MyLog, FormatString, ##__VA_ARGS__);\
}

#define M_LOG_ERROR(FormatString, ...)\
{\
	M_LOG_ERROR_TO(MyLog, FormatString, ##__VA_ARGS__);\
}
#define M_LOG_ERROR_IF(ShouldLog, FormatString, ...)\
{\
	M_LOG_ERROR_TO_IF(ShouldLog, MyLog, FormatString, ##__VA_ARGS__);\
}
#define M_LOG_ERROR_IF_FLAGS(LogFlags, FormatString, ...)\
{\
	M_LOG_ERROR_TO_IF_FLAGS(LogFlags, MyLog, FormatString, ##__VA_ARGS__);\
}

#define M_LOG_FATAL_ERROR(FormatString, ...)\
{\
	M_LOG_FATAL_ERROR_TO(MyLog, FormatString, ##__VA_ARGS__);\
}
#define M_LOG_FATAL_ERROR_IF(ShouldLog, FormatString, ...)\
{\
	M_LOG_FATAL_ERROR_TO_IF(ShouldLog, MyLog, FormatString, ##__VA_ARGS__);\
}
#define M_LOG_FATAL_ERROR_IF_FLAGS(LogFlags, FormatString, ...)\
{\
	M_LOG_FATAL_ERROR_TO_IF_FLAGS(LogFlags, MyLog, FormatString, ##__VA_ARGS__);\
}

#define M_NOT_IMPL(FormatString, ...)\
{\
	M_NOT_IMPL_TO(MyLog, FormatString, ##__VA_ARGS__);\
}

#define M_TO_BE_IMPL(FormatString, ...)\
{\
	M_TO_BE_IMPL_TO(MyLog, FormatString, ##__VA_ARGS__);\
}

#define M_IMPL_NOTE(FormatString, ...)\
{\
	M_IMPL_NOTE_TO(MyLog, FormatString, ##__VA_ARGS__);\
}

#define M_NOT_IMPL_RET(ReturnValue, FormatString, ...)\
{\
	M_NOT_IMPL_RET_TO(ReturnValue, MyLog, FormatString, ##__VA_ARGS__);\
}

#define M_TO_BE_IMPL_RET(ReturnValue, FormatString, ...)\
{\
	M_TO_BE_IMPL_RET_TO(ReturnValue, MyLog, FormatString, ##__VA_ARGS__);\
}

/**
* Marking body as empty
*/
#define M_EMPTY() { M_EMPTY_TO(MyLog); }
#define M_EMPTY_MSG(FormatString, ...) { M_EMPTY_MSG_TO(MyLog, FormatString, ##__VA_ARGS__); }

// ~Scoped helpres Begin (@warning: NEVER include implementation in brackets, because it will limit the scope)

#define M_CUSTOM_SCOPED_LOG_HELPER_CLASS_NAME(ClassNamePrefix) F##MyAutogenerated_##ClassNamePrefix##_ScopedLogHelper

/**
* Declares scoped helper class.
*/
#define M_DECLARE_CUSTOM_SCOPED_LOG_HELPER_CLASS(ClassNamePrefix, LogCategory, LogLevel)\
	class M_CUSTOM_SCOPED_LOG_HELPER_CLASS_NAME(ClassNamePrefix)\
	{\
	public:\
		M_CUSTOM_SCOPED_LOG_HELPER_CLASS_NAME(ClassNamePrefix)(const FString& InPrefixString)\
	:		PrefixString(InPrefixString)\
		{\
			M_LOG_CUSTOM_TO(MyLog, Log, TEXT("%s : Block entered"), *PrefixString);\
		}\
		~M_CUSTOM_SCOPED_LOG_HELPER_CLASS_NAME(ClassNamePrefix)()\
		{\
			M_LOG_CUSTOM_TO(MyLog, Log, TEXT("%s : Exiting block"), *PrefixString);\
		}\
	private:\
		FString PrefixString;\
	};

/**
* Helper class for logging enter/exit of the C++ block.
*/
#define M_SCOPED_LOG_HELPER_CLASS_TO(ClassNamePrefix, LogCategory) M_DECLARE_CUSTOM_SCOPED_LOG_HELPER_CLASS(ClassNamePrefix, LogCategory, Log);

/**
* Defining named log helper class instance.
* @note: Disabling the "local variable is shadowed" warning disable pragma is used, so unnamed scoped log helpers can be used inside nesting code blocks
* (scoped log helpers should never be accessed in C++ directly by name)*
*/
#define M_LOGFUNC_NAMED_STRING_TO(InName, LogCategory, InMessage)\
		__pragma(warning(push))\
		__pragma(warning(disable:4456))\
		M_SCOPED_LOG_HELPER_CLASS_TO(InName, LogCategory);\
		M_CUSTOM_SCOPED_LOG_HELPER_CLASS_NAME(InName) InName##_ScopedLogHelper {InMessage};\
		__pragma(warning(pop))
#define M_LOGFUNC_NAMED_TO(InName, LogCategory) M_LOGFUNC_NAMED_STRING_TO(InName, LogCategory, TEXT(""));
#define M_LOGFUNC_NAMED_MSG_TO(InName, LogCategory, FormatString, ...) M_LOGFUNC_NAMED_STRING_TO(InName, LogCategory, FString::Printf(FormatString, ##__VA_ARGS__));
#define M_LOGBLOCK_NAMED_TO(InName, LogCategory, FormatString, ...) M_LOGFUNC_NAMED_MSG_TO(InName, LogCategory, FormatString, ##__VA_ARGS__);

#define M_LOGFUNC_STRING_TO(LogCategory, InMessage) M_LOGFUNC_NAMED_STRING_TO(Unnamed, LogCategory, InMessage);
#define M_LOGFUNC_TO(LogCategory) M_LOGFUNC_STRING_TO(LogCategory, TEXT(""));
#define M_LOGFUNC_MSG_TO(LogCategory, FormatString, ...) M_LOGFUNC_STRING_TO(LogCategory, FString::Printf(FormatString, ##__VA_ARGS__));
#define M_LOGBLOCK_TO(LogCategory, FormatString, ...) M_LOGFUNC_MSG_TO(LogCategory, FormatString, ##__VA_ARGS__);
// ~Scoped helpers End

#define M_LOGFUNC() M_LOGFUNC_TO(MyLog);
#define M_LOGFUNC_MSG(FormatString, ...) M_LOGFUNC_MSG_TO(MyLog, FormatString, ##__VA_ARGS__);
#define M_LOGBLOCK(FormatString, ...) M_LOGBLOCK_TO(MyLog, FormatString, ##__VA_ARGS__);