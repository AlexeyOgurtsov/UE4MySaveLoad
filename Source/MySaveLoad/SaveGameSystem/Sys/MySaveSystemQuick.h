#pragma once

/**
* Quick implementation of the Save system.
*/

#include "SaveGameSystem/Sys/Impl/Sys/IMySaveSystemInternal.h"
#include "MySaveSystemQuick.generated.h"

class IMySaveableHandle;

UCLASS()
class UMySaveSystemQuick : 
	public UObject, 
	public IMySaveSystemInternal
{
	GENERATED_BODY()

public:
	// ~IMySaveSystem Begin
	virtual TScriptInterface<IMySaveableHandle> CreateSaveableHandle(TScriptInterface<IMySaveable> InObject) override;
	virtual void NotifyObjectDestructed(TScriptInterface<IMySaveableHandle> InSaveableHandle) override;
	// ~IMySaveSystem End

	// ~IMySaveSystemInternal Begin
	virtual const TArray<TScriptInterface<IMySaveable>>& GetSaveableObjects() const override { return SaveableObjects; }
	virtual const TArray<FName>& GetStaticDestructedObjects() const override { return StaticDestructedObjects; }
	// ~IMySaveSystemInternal End

private:
	void RegisterSaveableObject(TScriptInterface<IMySaveableHandle> InSaveable);

	UPROPERTY()
	TArray<TScriptInterface<IMySaveable>> SaveableObjects;

	UPROPERTY()
	TArray<FName> StaticDestructedObjects;
};