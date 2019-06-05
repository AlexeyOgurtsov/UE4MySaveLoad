#pragma once

/**
* Calls from the IMySaveable interface to be redirected to this object typically.
*
* WARNING!!! Should NOT implement the IMySaveable, however (because in that case it will be enumerated by the system)!
*/

#include "IMySaveable.h"
#include "MySaveableObjectHelper.generated.h"

class IMySaveSystem;

USTRUCT()
struct FMySaveableObjectHelper
{
	GENERATED_BODY()

	FMySaveableObjectHelper();
	FMySaveableObjectHelper(TScriptInterface<IMySaveable> InSaveableObject, IMySaveSystem* InSys);

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

	void AssignData(UMySaverLoaderBase* InSender, UPerObjectSaveLoadDataBase* Data);
	UPerObjectSaveLoadDataBase* GetData(UMySaverLoaderBase* InSender) const { return Data; }
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