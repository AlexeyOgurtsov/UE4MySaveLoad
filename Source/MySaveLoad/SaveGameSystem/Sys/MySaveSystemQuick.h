#pragma once

/**
* Quick implementation of the Save system.
*/

#include "SaveGameSystem/Sys/Impl/Sys/IMySaveSystemInternal.h"
#include "MySaveSystemQuick.generated.h"

UCLASS()
class UMySaveSystemQuick : 
	public UObject, 
	public IMySaveSystemInternal
{
	GENERATED_BODY()

public:
	// ~IMySaveSystem Begin
	virtual void NotifyObjectDestructed(TScriptInterface<IMySaveable> InObject) override;
	// ~IMySaveSystem End

	// ~IMySaveSystemInternal Begin
	virtual const TArray<TScriptInterface<IMySaveable>>& GetSaveableObjects() const override { return SaveableObjects; }
	virtual void RegisterSaveableObject(TScriptInterface<IMySaveable> InSaveable) override;
	virtual const TArray<FName>& GetStaticDestructedObjects() const override { return StaticDestructedObjects; }
	// ~IMySaveSystemInternal End

private:
	UPROPERTY()
	TArray<TScriptInterface<IMySaveable>> SaveableObjects;

	UPROPERTY()
	TArray<FName> StaticDestructedObjects;
};