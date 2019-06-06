#include "../MySaveableObjectHelper.h"
#include "../IMySaveable.h"
#include "../MySaveableUtils.h"
#include "../IMySaveSystem.h"
#include "../Util/MySaveArchive.h"

#include "Util/Core/LogUtilLib.h"

#include "Serialization/Archive.h"

// @TODO 2: How properties to be setup in editor?

UMySaveableObjectHelper::UMySaveableObjectHelper()
{
	InitPrefixString();
}

UMySaveableObjectHelper* UMySaveableObjectHelper::NewSaveableHelper(TScriptInterface<IMySaveable> InSaveableObject, IMySaveSystem* InSys)
{
	check(InSaveableObject);
	check(InSys);

	UMySaveableObjectHelper* Obj = NewObject<UMySaveableObjectHelper>(Cast<UObject>(InSys), UMySaveableObjectHelper::StaticClass());
	checkf(Obj, TEXT("NewObject must succeed!"));

	Obj->SaveableObject = InSaveableObject;
	Obj->Sys = InSys;

	Obj->InitPrefixString();

	return Obj;
}

void UMySaveableObjectHelper::InitPrefixString()
{
	TArray<FStringFormatArg> FormatArgs;
	FormatArgs.Add(SaveableObject.GetObject()->GetName()); 
	FormatArgs.Add(SaveableObject->GetUniqueName());
	FormatArgs.Add(SaveableObject.GetObject()->GetClass()->GetName());
	PrefixString = FString::Format(TEXT("FMySaveableObjectHelper for object \"{0}\" with UniqueName \"{1}\" of class \"{2}\":"), FormatArgs);
}

IMySaveSystem* UMySaveableObjectHelper::GetSys() const
{
	return Sys;
}

void UMySaveableObjectHelper::AssignData(UMySaverLoaderBase* const InSender, UPerObjectSaveLoadDataBase* const InData)
{		
	UE_LOG(MyLog, Log, TEXT("%s AssignData"), *PrefixString);
	Data = InData;
}

void UMySaveableObjectHelper::Notify_BeginDestroy()
{
	UE_LOG(MyLog, Log, TEXT("%s Notify_BeginDestroy is called"), *PrefixString);
	// WARNING!!! Here we must notify about ANY object destruction (NOT only for NON-created dynamically),
	// the subsystem must determine by itself, whether it should do anything with this object	
	Sys->NotifyObjectDestructed(GetSaveableObject());
}

void UMySaveableObjectHelper::Default_Serialize(FArchive& Ar)
{
	UE_LOG(MyLog, Log, TEXT("%s Default_Serialize is called"), *PrefixString);
	UMySaveableUtils::SerializeObjectData(Ar, GetSaveableObject().GetObject());
}

void UMySaveableObjectHelper::Default_AllObjectsLoaded(FArchive& Ar)
{
	// Nothing is to do here yet
}