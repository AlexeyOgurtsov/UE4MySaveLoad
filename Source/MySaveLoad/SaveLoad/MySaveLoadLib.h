#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Util/Core/Log/MyLoggingTypes.h"
#include "MySaveLoadLib.generated.h"

class IMySaveLoadSystem;

UCLASS()
class UMySaveLoadLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	* Synchronously saves game into the given slot for the given user.
	* @returns: true if successfully saved.
	*/
	UFUNCTION(BlueprintCallable, Category = SaveLoad)
	static bool Save(TScriptInterface<IMySaveLoadSystem> InSys, const FString& InSlotName, int32 InUserIndex, ELogFlags InLogFlags = ELogFlags::LogEverSuccess);

	/**
	* Synchronously loads game from the given slot for the given user.
	* @returns: true if successfully saved.
	*/
	UFUNCTION(BlueprintCallable, Category = SaveLoad)
	static bool Load(TScriptInterface<IMySaveLoadSystem> InSys, const FString& InSlotName, int32 InUserIndex, ELogFlags InLogFlags = ELogFlags::LogEverSuccess);
};