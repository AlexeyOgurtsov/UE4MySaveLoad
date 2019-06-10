#include "../../MySaveableHandleObject.h"
#include "SaveLoad/IMySaveable.h"
#include "SaveLoad/MySaveableUtils.h"
#include "SaveLoad/IMySaveLoadSystem.h"
#include "SaveLoad/Util/MySaveArchive.h"

#include "SaveLoad/Util/SaveLoadLogUtilLib.h"

#include "Serialization/Archive.h"

// @TODO 2: How properties to be setup in editor?

UMySaveableHandleObject::UMySaveableHandleObject()
{
	InitPrefixString();
}

UMySaveableHandleObject* UMySaveableHandleObject::CreateSaveableHandleDefaultSubobject(TScriptInterface<IMySaveable> InSaveable, IMySaveLoadSystem* InSys)
{
	check(InSaveable);
	check(InSys);

	UMySaveableHandleObject* Obj = Cast<UObject>(InSys)->CreateDefaultSubobject<UMySaveableHandleObject>(TEXT("SaveableHandle"));
	checkf(Obj, TEXT("NewObject must succeed!"));

	Obj->Saveable = InSaveable;
	Obj->Sys = InSys;

	Obj->InitPrefixString();

	return Obj;
}

void UMySaveableHandleObject::BeginDestroy()
{
	SL_LOGFUNC();
	checkNoRecursion();

	Super::BeginDestroy();
	// WARNING!!! Here we must notify about ANY object destruction (NOT only for NON-created dynamically),
	// the subsystem must determine by itself, whether it should do anything with this object	
	Sys->NotifyObjectDestructed(this);
}

void UMySaveableHandleObject::InitPrefixString()
{
	PrefixString = Saveable->SaveLoad_GetPrefixString(FString(TEXT("UMySaveableHandleObject")));
}

IMySaveLoadSystem* UMySaveableHandleObject::GetSys() const
{
	return Sys;
}

void UMySaveableHandleObject::SaveLoad_AssignData(UMySaverLoaderBase* const InSender, UPerObjectSaveLoadDataBase* const InData)
{		
	SL_LOGFUNC();
	Data = InData;
}