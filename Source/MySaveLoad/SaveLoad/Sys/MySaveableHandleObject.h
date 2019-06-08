#pragma once

/**
* Calls from the IMySaveable interface to be redirected to this object typically.
*
* @warning: Does NOT implement the IMySaveable, however (because in that case it will be enumerated by the system)!
*/

#include "SaveLoad/IMySaveableHandle.h"
#include "SaveLoad/MySaveableTypes.h"
#include "MySaveableHandleObject.generated.h"

class UPerObjectSaveLoadDataBase;
class UMySaverLoaderBase;

class IMySaveLoadSystem;
class IMySaveable;

UCLASS()
class UMySaveableHandleObject : 
	public UObject
,	public IMySaveableHandle
{
	GENERATED_BODY()

public:
	UMySaveableHandleObject();

	/**
	* Creates saveable handle using the CreateDefaultSubobject, so must always be called in the constructor only.
	*/
	static UMySaveableHandleObject* CreateSaveableHandleDefaultSubobject(TScriptInterface<IMySaveable> InSaveable, IMySaveLoadSystem* InSys);

	// ~Helper getters Begin
	IMySaveLoadSystem* GetSys() const;
	// ~Helper getters End

	// ~Link to owner Begin
	TScriptInterface<IMySaveable> GetSaveableObject() const { return Saveable; }
	// ~Link to owner End

	// ~Object notification methods Begin
	void Notify_BeginDestroy();
	// ~Object notification methods End

	virtual TScriptInterface<IMySaveable> SaveLoad_GetSaveable() const override { return Saveable; }

	virtual const FMySaveablePerClassProps& SaveLoad_GetClassProps() const override { return ClassProps; }
	virtual const FMySaveableStaticProps& SaveLoad_GetStaticProps() const override { return StaticProps; }

	// ~SaveLoad data Begin
	/**
	* Assign data (to be called from loader only!).
	*/
	virtual void SaveLoad_AssignData(UMySaverLoaderBase* InSender, UPerObjectSaveLoadDataBase* Data) override;
	virtual UPerObjectSaveLoadDataBase* SaveLoad_GetData(UMySaverLoaderBase* InSender) const override { return Data; }
	// ~SaveLoad data End

private:
	void InitPrefixString();

	UPROPERTY()
	FString PrefixString;

	UPROPERTY()
	TScriptInterface<IMySaveable> Saveable = nullptr;

	IMySaveLoadSystem* Sys = nullptr;

	UPROPERTY()
	FMySaveablePerClassProps ClassProps;

	UPROPERTY()
	FMySaveableStaticProps StaticProps;
	
	UPROPERTY()
	UPerObjectSaveLoadDataBase* Data = nullptr;
};