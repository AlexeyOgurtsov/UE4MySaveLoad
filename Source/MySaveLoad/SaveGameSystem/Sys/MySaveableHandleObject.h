#pragma once

/**
* Calls from the IMySaveable interface to be redirected to this object typically.
*
* @warning: Does NOT implement the IMySaveable, however (because in that case it will be enumerated by the system)!
*/

#include "SaveGameSystem/IMySaveableHandle.h"
#include "SaveGameSystem/MySaveableTypes.h"
#include "MySaveableHandleObject.generated.h"

class UPerObjectSaveLoadDataBase;
class UMySaverLoaderBase;

class IMySaveSystem;
class IMySaveable;

UCLASS()
class UMySaveableHandleObject : 
	public UObject
,	public IMySaveableHandle
{
	GENERATED_BODY()

public:
	UMySaveableHandleObject();

	static UMySaveableHandleObject* NewSaveableHandleObject(TScriptInterface<IMySaveable> InSaveable, IMySaveSystem* InSys);

	// ~Helper getters Begin
	IMySaveSystem* GetSys() const;
	// ~Helper getters End

	// ~Link to owner Begin
	TScriptInterface<IMySaveable> GetSaveableObject() const { return Saveable; }
	// ~Link to owner End

	// ~Object notification methods Begin
	void Notify_BeginDestroy();
	// ~Object notification methods End

	// ~SaveLoad data Begin
	/**
	* Assign data (to be called from loader only!).
	*/
	virtual void SaveLoad_AssignData(UMySaverLoaderBase* InSender, UPerObjectSaveLoadDataBase* Data) override;
	virtual UPerObjectSaveLoadDataBase* SaveLoad_GetData(UMySaverLoaderBase* InSender) const override { return Data; }
	// ~SaveLoad data End

	// ~IMySaveable mirrored methods Begin
	const FMySaveablePerClassProps& GetClassProps() const { return ClassProps; }
	const FMySaveableStaticProps& GetStaticProps() const { return StaticProps; }
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
	TScriptInterface<IMySaveable> Saveable = nullptr;

	IMySaveSystem* Sys = nullptr;

	UPROPERTY()
	FMySaveablePerClassProps ClassProps;

	UPROPERTY()
	FMySaveableStaticProps StaticProps;
	
	UPROPERTY()
	UPerObjectSaveLoadDataBase* Data = nullptr;
};