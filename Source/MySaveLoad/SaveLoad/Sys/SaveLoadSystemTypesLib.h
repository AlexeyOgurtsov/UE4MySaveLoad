#pragma once

#include "SaveLoadSystemTypesLib.generated.h"

USTRUCT()
struct FSaveLoadSystemProps
{
	GENERATED_BODY()

	/**
	* Should we reload level when we loading a game from save?
	*/
	UPROPERTY()
	bool bAlwaysReloadLevel = true;
};

/**
* Public library to work with the save load system types.
*/
UCLASS()
class USaveLoadSystemTypesLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
};