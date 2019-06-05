#pragma once

/**
* State to be used by UMySaveBase exclusively (not while loading).
*/

#include "MySaveLoadState.generated.h"

class IMySaveable;
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

	UPROPERTY()
	TArray<TScriptInterface<IMySaveable>> GlobalObjects;

	UPROPERTY()
	TArray<TScriptInterface<IMySaveable>> WorldObjects;

	UPROPERTY()
	TArray<FName> DestructedObjects;

private:
};