#pragma once

/**
* Calls from the IMySaveable interface to be redirected to this object typically.
*
* WARNING!!! Should NOT implement the IMySaveable, however (because in that case it will be enumerated by the system)!
*/

#include "IMySaveable.h"
#include "MySaveableObjectHelper.generated.h"

class IMySaveSystem;

UCLASS()
class UMySaveableObjectHelper : public UObject
{
	GENERATED_BODY()

public:
	UMySaveableObjectHelper();

	static UMySaveableObjectHelper* NewSaveableHelper(TScriptInterface<IMySaveable> InSaveableObject, IMySaveSystem* InSys);

	// ~Helper getters Begin
	IMySaveSystem* GetSys() const;
	// ~Helper getters End

	// ~Link to owner Begin
	TScriptInterface<IMySaveable> GetSaveableObject() const { return SaveableObject; }
	// ~Link to owner End

	// ~Object notification methods Begin
	void Notify_BeginDestroy();
	// ~Object notification methods End

	// ~IMySaveable mirrored methods Begin
	const FMySaveablePerClassProps& GetClassProps() const { return ClassProps; }
	const FMySaveableStaticProps& GetStaticProps() const { return StaticProps; }

	void AssignData(UMySaverLoaderBase* const InSender, UPerObjectSaveLoadDataBase* const Data);
	UPerObjectSaveLoadDataBase* GetData(UMySaverLoaderBase* const InSender) const { return Data; }
	// ~IMySaveable mirrored methods End

	// ~IMySaveable default Begin
	void Default_Serialize(FArchive& Ar);
	void Default_AllObjectsLoaded(FArchive& Ar);
	// ~IMySaveable default End

private:
	void InitPrefixString();

	UPROPERTY()
	FString PrefixString;

	UPROPERTY()
	TScriptInterface<IMySaveable> SaveableObject = nullptr;

	IMySaveSystem* Sys = nullptr;

	UPROPERTY()
	FMySaveablePerClassProps ClassProps;

	UPROPERTY()
	FMySaveableStaticProps StaticProps;
	
	UPROPERTY()
	UPerObjectSaveLoadDataBase* Data = nullptr;
};