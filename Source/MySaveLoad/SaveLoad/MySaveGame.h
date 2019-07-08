#pragma once

#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

UCLASS()
class UMySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<uint8> Data;
};