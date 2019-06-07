#pragma once

/**
* Quick implementation of the Save system.
*/

#include "SaveLoad/Sys/Impl/Sys/IMySaveLoadSystemInternal.h"
#include "MySaveLoadSystemQuick.generated.h"

class IMySaveableHandle;

UCLASS()
class UMySaveLoadSystemQuick : 
	public UObject, 
	public IMySaveLoadSystemInternal
{
	GENERATED_BODY()

public:
	// ~IMySaveLoadSystem Begin
	virtual TScriptInterface<IMySaveableHandle> CreateSaveableHandle(TScriptInterface<IMySaveable> InObject) override;
	virtual void NotifyObjectDestructed(TScriptInterface<IMySaveableHandle> InSaveableHandle) override;
	// ~IMySaveLoadSystem End

	// ~IMySaveLoadSystemInternal Begin
	virtual const TArray<TScriptInterface<IMySaveable>>& GetSaveableObjects() const override { return SaveableObjects; }
	virtual const TArray<FName>& GetStaticDestructedObjects() const override { return StaticDestructedObjects; }
	// ~IMySaveLoadSystemInternal End

private:
	void RegisterSaveableObject(TScriptInterface<IMySaveableHandle> InSaveable);

	UPROPERTY()
	TArray<TScriptInterface<IMySaveable>> SaveableObjects;

	UPROPERTY()
	TArray<FName> StaticDestructedObjects;
};