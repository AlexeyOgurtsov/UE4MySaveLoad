#pragma once

#include "Logging/LogMacros.h"
#include "MyLoggingTypes.generated.h"

UENUM(BlueprintType, Meta=(Bitflags))
enum class ELogFlags : uint8
{
	None              = 0          UMETA(Name="None"),
	LogEverSuccess    = 1 << 1     UMETA(Name="LogEverSuccess", Description="Should we log even successful operations"),
	DisableLog        = 1 << 2     UMETA(Name="DisableLog", Description="Should we disabled ever logging of fails")
};
ENUM_CLASS_FLAGS(ELogFlags);

UCLASS()
class UMyLoggingTypes : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = MyLoggingTypes)
	static void CheckFlagsValid(ELogFlags InFlags);

	UFUNCTION(BlueprintPure, Category = MyLoggingTypes)
	static bool AreFlagsValid(ELogFlags InFlags);

	UFUNCTION(BlueprintPure, Category = MyLoggingTypes)
	static bool ShouldLog(ELogFlags InFlags, bool bErrorOrWarn);

	static bool ShouldLogVerbosity(ELogFlags InFlags, ELogVerbosity::Type InVerbosity);
	static bool IsVerbosityWarnOrError(ELogVerbosity::Type InVerbosity);
};