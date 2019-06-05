#pragma once

/**
* Utils of the Save load system, to be used internally (never should be used by user serialization code).
*/

#include "../Format/MySaveTypes.h"
#include "MySaveLoadSystemUtils.generated.h"

class IMySaveable;
class UMySaverLoaderBase;
class UMySaveLoadState;
class IMySaveable;

UCLASS()
class UMySaveLoadSystemUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void CheckSavedWorldValid(const FMySavedWorld* InWorld);

	static bool IsSavedWorldValid(const FMySavedWorld* InWorld, bool bInLogged = true);

	static bool IsSavedWorldMetaValid(const FMySaveFormatMeta* InMeta, bool bInLogged = true);

	static bool IsSavedObjectValid(UMySaverLoaderBase* InSender, const FMySavedObject* InSavedObj, const FMySavedWorld* InWorld, bool bInLogged = true);

	static bool IsSavedClassIndexValid(UMySaverLoaderBase* InSender, const FMySavedWorld* InWorld, int32 InClassIndex, bool bInLogged = true, const FString& LogPrefix = FString{});

	static void PrepareFormatMetaSaveStruct(UMySaverLoaderBase* InSender, FMySaveFormatMeta* InSavedFormatMeta);

	static void PrepareWorldInfoSaveStruct(UMySaverLoaderBase* InSender, FMySavedWorldInfo* InSavedWorldInfo, const UMySaveLoadState* InSaveLoadState);

	static void PrepareObjectDestructStruct(UMySaverLoaderBase* InSender, FMySavedDestruct* InSavedDestruct, const FName& InUniqueName);

	static void PrepareDestructedObjects_FromLoadState(UMySaverLoaderBase* InSender, TArray<FMySavedDestruct>* InSavedDestructedObjects, const TArray<FName>& InDestructedObjects);

	/**
	* Get saved struct from object WITHOUT its data (which are to be serialized later).
	*/
	static void PrepareObjectSaveStruct(UMySaverLoaderBase* InSender, FMySavedObject* InSavedObj, TScriptInterface<IMySaveable> InObj);

	static void PrepareObjectSaveData(UMySaverLoaderBase* InSender, FMySavedObject* InSavedObj, TScriptInterface<IMySaveable> InObj);

	static void PrepareClassSaveStruct(UMySaverLoaderBase* InSender, FMySavedClass* InSavedClass, UClass* InClass);

	static void PrepareSaveClasses_FromLoadState(UMySaverLoaderBase* InSender, FMySavedWorld* InSavedWorld, const UMySaveLoadState* InSaveLoadState);

	static void SaveLoadStateToSaveStruct(UMySaverLoaderBase* InSender, FMySavedWorld* InSavedWorld, const UMySaveLoadState* InSaveLoadState);

	static void PrepareSaveObjects_FromLoadState(UMySaverLoaderBase* InSender, TArray<FMySavedObject>* InSavedObjects, const TArray<TScriptInterface<IMySaveable>>& InObjects);
};
