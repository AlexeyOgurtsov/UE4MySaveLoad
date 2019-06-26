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
	virtual void Load(FArchive& Ar) override;
	virtual void Save(FArchive& Ar) override;

	virtual TScriptInterface<IMySaveableHandle> CreateSaveableHandle_Implementation(const TScriptInterface<IMySaveable>& InSaveable) override;
	virtual void NotifySaveableDestructed_Implementation(const TScriptInterface<IMySaveableHandle>& InSaveableHandle) override;
	// ~IMySaveLoadSystem End

	// ~IMySaveLoadSystemInternal Begin
	virtual TArray<TScriptInterface<IMySaveableHandle>>::TConstIterator CreateSaveableHandleIterator() const override { return SaveableHandles.CreateConstIterator(); }
	virtual int32 NumSaveables() const override { return SaveableHandles.Num(); }
	virtual const TArray<FName>& GetStaticDestructedObjects() const override { return StaticDestructedObjects; }
	// ~IMySaveLoadSystemInternal End

private:
	void RegisterSaveableObject(TScriptInterface<IMySaveableHandle> InSaveable);
	void UnregisterSaveableObjectChecked(TScriptInterface<IMySaveableHandle> InSaveable);

	/**
	* Contains only handles with SaveLoad enabled.
	*/
	UPROPERTY()
	TArray<TScriptInterface<IMySaveableHandle>> SaveableHandles;

	UPROPERTY()
	TArray<FName> StaticDestructedObjects;
};