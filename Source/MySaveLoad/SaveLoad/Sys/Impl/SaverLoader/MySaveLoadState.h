#pragma once

/**
* State to be used by UMySaveBase exclusively (not while loading).
*/

#include "MySaveLoadState.generated.h"

class IMySaveable; // @TODO: Remove
class IMySaveableHandle;
UWorld;

UCLASS()
class UMySaveLoadState : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString MapName;

	/**
	* All saveable classes (index is id).
	*/
	UPROPERTY()
	TArray<UClass*> Classes;

	UPROPERTY(Meta=(DeprecatedProperty, DeprecationMessage="Use GlobalSaveableHandles"))
	TArray<TScriptInterface<IMySaveable>> GlobalObjects;

	UPROPERTY(Meta=(DeprecatedProperty, DeprecationMessage="Use WorldSaveableHandles"))
	TArray<TScriptInterface<IMySaveable>> WorldObjects;

	UPROPERTY()
	TArray<TScriptInterface<IMySaveableHandle>> GlobalSaveableHandles;

	UPROPERTY()
	TArray<TScriptInterface<IMySaveableHandle>> WorldSaveableHandles;

	UPROPERTY()
	TArray<FName> StaticDestructedObjects;

private:
};