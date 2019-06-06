#include "../../MySaveableHandleObject.h"
#include "SaveGameSystem/IMySaveable.h"
#include "SaveGameSystem/MySaveableUtils.h"
#include "SaveGameSystem/IMySaveSystem.h"
#include "SaveGameSystem/Util/MySaveArchive.h"

#include "Util/Core/LogUtilLib.h"

#include "Serialization/Archive.h"

// @TODO 2: How properties to be setup in editor?

UMySaveableHandleObject::UMySaveableHandleObject()
{
	InitPrefixString();
}

UMySaveableHandleObject* UMySaveableHandleObject::NewSaveableHandleObject(TScriptInterface<IMySaveable> const InSaveable, IMySaveSystem* const InSys)
{
	check(InSaveable);
	check(InSys);

	UMySaveableHandleObject* Obj = NewObject<UMySaveableHandleObject>(Cast<UObject>(InSys), UMySaveableHandleObject::StaticClass());
	checkf(Obj, TEXT("NewObject must succeed!"));

	Obj->Saveable = InSaveable;
	Obj->Sys = InSys;

	Obj->InitPrefixString();

	return Obj;
}

void UMySaveableHandleObject::InitPrefixString()
{
	PrefixString = Saveable->SaveLoad_GetPrefixString(FString(TEXT("UMySaveableHandleObject")));
}

IMySaveSystem* UMySaveableHandleObject::GetSys() const
{
	return Sys;
}

void UMySaveableHandleObject::AssignData(UMySaverLoaderBase* const InSender, UPerObjectSaveLoadDataBase* const InData)
{		
	UE_LOG(MyLog, Log, TEXT("%s AssignData"), *PrefixString);
	Data = InData;
}

void UMySaveableHandleObject::Notify_BeginDestroy()
{
	UE_LOG(MyLog, Log, TEXT("%s Notify_BeginDestroy is called"), *PrefixString);
	// WARNING!!! Here we must notify about ANY object destruction (NOT only for NON-created dynamically),
	// the subsystem must determine by itself, whether it should do anything with this object	
	Sys->NotifyObjectDestructed(GetSaveableObject());
}

void UMySaveableHandleObject::Default_Serialize(FArchive& Ar)
{
	UE_LOG(MyLog, Log, TEXT("%s Default_Serialize is called"), *PrefixString);
	UMySaveableUtils::SerializeObjectData(Ar, GetSaveableObject().GetObject());
}

void UMySaveableHandleObject::Default_AllObjectsLoaded(FArchive& Ar)
{
	// Nothing is to do here yet
}