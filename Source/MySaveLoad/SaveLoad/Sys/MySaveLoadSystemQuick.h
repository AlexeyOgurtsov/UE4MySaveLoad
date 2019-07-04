#pragma once

/**
* Quick implementation of the Save system.
*/

#include "SaveLoad/Sys/Impl/Sys/IMySaveLoadSystemInternal.h"
#include "MySaveLoadSystemQuick.generated.h"

class IMySaveableHandle;

class UWorld;

/**
* Contains all parameters to be passed during creation of the save load system.
*/
USTRUCT(BlueprintType)
struct FQuickSaveLoadSystemInitializer
{
	GENERATED_BODY()

	/**
	* The world we should operate upon.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = World)
	UWorld* World = nullptr;

	FQuickSaveLoadSystemInitializer() {}
	FQuickSaveLoadSystemInitializer(UWorld* InWorld)
	:	World(InWorld)
	{
		const TCHAR* const FUNC_DESC = TEXT("save load system initializer");
		checkf(InWorld, TEXT("Valid UWorld pointer must be passed to the %s"), FUNC_DESC);
	}
};

UCLASS()
class UMySaveLoadSystemQuick : 
	public UObject, 
	public IMySaveLoadSystemInternal
{
	GENERATED_BODY()

public:
	// ~Creation Begin
	UFUNCTION(BlueprintCallable, Category = QuickSaveLoadSystem)
	static UMySaveLoadSystemQuick* NewQuickSaveLoadSystem(UObject* InOuter, const FQuickSaveLoadSystemInitializer& InInitializer);

	UFUNCTION(BlueprintCallable, Category = QuickSaveLoadSystem)
	static UMySaveLoadSystemQuick* CreateDefaultQuickSaveLoadSystem(UObject* InOuter, const FQuickSaveLoadSystemInitializer& InInitializer);
	// ~Creation End

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
	/**
	* See description in the initializer.
	*/
	UPROPERTY()
	UWorld* World = nullptr;

	// ~Initialization Begin
	/**
	* Initializes the quick Save Load system.
	*/
	void InitializeQuickSaveLoad(UObject* InOuter, const FQuickSaveLoadSystemInitializer& InInitializer);
	// ~Initialization End

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
