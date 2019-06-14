#pragma once

/**
* Utils of the Save load system, to be used internally (never should be used by user serialization code).
*/

#include "../Format/MySaveTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MySaveLoadSystemUtils.generated.h"

class IMySaveableHandle;
class UMySaverLoaderBase;
class UMySaveLoadState;
class IMySaveable;

UCLASS()
class UMySaveLoadSystemUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void CheckSavedWorldValid(const FMySavedWorld* const InWorld);

	static bool IsSavedWorldValid(const FMySavedWorld* const InWorld, bool const bInLogged = true);

	static bool IsSavedWorldMetaValid(const FMySaveFormatMeta* const InMeta, bool const bInLogged = true);

	static bool IsSavedObjectValid(UMySaverLoaderBase* const InSender, const FMySavedObject* const InSavedObj, const FMySavedWorld* const InWorld, bool const bInLogged = true);

	static bool IsSavedClassIndexValid(UMySaverLoaderBase* const InSender, const FMySavedWorld* const InWorld, int32 const InClassIndex, bool const bInLogged = true, const FString& LogPrefix = FString{});

	static void PrepareFormatMetaSaveStruct(UMySaverLoaderBase* const InSender, FMySaveFormatMeta* const InSavedFormatMeta);

	static void PrepareWorldInfoSaveStruct(UMySaverLoaderBase* const InSender, FMySavedWorldInfo* const InSavedWorldInfo, const UMySaveLoadState* const InSaveLoadState);

	static void PrepareObjectDestructStruct(UMySaverLoaderBase* const InSender, FMySavedDestruct* const InSavedDestruct, const FName& InUniqueName);

	static void PrepareDestructedObjects_FromLoadState(UMySaverLoaderBase* const InSender, TArray<FMySavedDestruct>* const InSavedDestructedObjects, const TArray<FName>& InDestructedObjects);

	/**
	* Get saved struct from object WITHOUT its data (which are to be serialized later).
	*/
	static void PrepareObjectSaveStruct(UMySaverLoaderBase* const InSender, FMySavedObject* const InSavedObj, TScriptInterface<IMySaveableHandle> InSaveableHandle);

	static void PrepareObjectSaveData(UMySaverLoaderBase* const InSender, FMySavedObject* const InSavedObj, TScriptInterface<IMySaveableHandle> InSaveableHandle);

	static void PrepareClassSaveStruct(UMySaverLoaderBase* const InSender, FMySavedClass* const InSavedClass, UClass* const InClass);

	static void PrepareSaveClasses_FromLoadState(UMySaverLoaderBase* const InSender, FMySavedWorld* const InSavedWorld, const UMySaveLoadState* const InSaveLoadState);

	static void SaveLoadStateToSaveStruct(UMySaverLoaderBase* const InSender, FMySavedWorld* const InSavedWorld, const UMySaveLoadState* const InSaveLoadState);

	static void PrepareSaveObjects_FromLoadState(UMySaverLoaderBase* const InSender, TArray<FMySavedObject>* const InSavedObjects, const TArray<TScriptInterface<IMySaveableHandle>>& InObjects);
};
