#include "../MySaveableObjectHelper.h"
#include "../IMySaveable.h"
#include "../MySaveableUtils.h"
#include "../IMySaveSystem.h"
#include "../Util/MySaveArchive.h"

#include "Util/Core/LogUtilLib.h"

#include "Serialization/Archive.h"

// @TODO 2: How properties to be setup in editor?

FMySaveableObjectHelper::FMySaveableObjectHelper()
{
	InitPrefixString();
}

FMySaveableObjectHelper::FMySaveableObjectHelper(TScriptInterface<IMySaveable> InSaveableObject, IMySaveSystem* InSys)
{
	check(InSaveableObject);
	check(InSys);

	SaveableObject = InSaveableObject;
	Sys = InSys;

	InitPrefixString();
}

void FMySaveableObjectHelper::InitPrefixString()
{
	TArray<FStringFormatArg> FormatArgs;
	FormatArgs.Add(*SaveableObject.GetObject()->GetName()); 
	FormatArgs.Add(UMySaveableUtils::GetUniqueName(SaveableObject).ToString());
	FormatArgs.Add(*SaveableObject.GetObject()->GetClass()->GetName());
	PrefixString = FString::Format(TEXT("FMySaveableObjectHelper for object \"{0}\" with UniqueName \"{1}\" of class \"{2}\":"), FormatArgs);
}

IMySaveSystem* FMySaveableObjectHelper::GetSys() const
{
	return Sys;
}

void FMySaveableObjectHelper::AssignData(UMySaverLoaderBase* InSender, UPerObjectSaveLoadDataBase* InData)
{		
	UE_LOG(MyLog, Log, TEXT("%s AssignData"), *PrefixString);
	Data = InData;
}

void FMySaveableObjectHelper::Notify_BeginDestroy()
{
	UE_LOG(MyLog, Log, TEXT("%s Notify_BeginDestroy is called"), *PrefixString);
	// WARNING!!! Here we must notify about ANY object destruction (NOT only for NON-created dynamically),
	// the subsystem must determine by itself, whether it should do anything with this object	
	Sys->NotifyObjectDestructed(GetSaveableObject());
}

void FMySaveableObjectHelper::Default_Serialize(FArchive& Ar)
{
	UE_LOG(MyLog, Log, TEXT("%s Default_Serialize is called"), *PrefixString);
	UMySaveableUtils::SerializeObjectData(Ar, GetSaveableObject().GetObject());
}

void FMySaveableObjectHelper::Default_AllObjectsLoaded(FArchive& Ar)
{
	// Nothing is to do here yet
}