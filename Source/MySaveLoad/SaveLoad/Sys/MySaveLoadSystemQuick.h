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
	virtual const TArray<TScriptInterface<IMySaveable>>& GetSaveableObjects() const override { return SaveableObjects_DEPRECATED; } // @TODO: Remove
	virtual TArray<TScriptInterface<IMySaveableHandle>>::TConstIterator CreateSaveableHandleIterator() const override { return SaveableHandles.CreateConstIterator(); }
	virtual int32 NumSaveables() const override { return SaveableHandles.Num(); }
	virtual const TArray<FName>& GetStaticDestructedObjects() const override { return StaticDestructedObjects; }
	// ~IMySaveLoadSystemInternal End

private:
	void RegisterSaveableObject(TScriptInterface<IMySaveableHandle> InSaveable);
	void UnregisterSaveableObjectChecked(TScriptInterface<IMySaveableHandle> InSaveable);

	UPROPERTY(meta = (DeprecatedProperty, DeprecationMessage="SaveableHandles now stores handles"))
	TArray<TScriptInterface<IMySaveable>> SaveableObjects_DEPRECATED; // @TODO: Remove

	/**
	* Contains only handles with SaveLoad enabled.
	*/
	UPROPERTY()
	TArray<TScriptInterface<IMySaveableHandle>> SaveableHandles;

	UPROPERTY()
	TArray<FName> StaticDestructedObjects;
};