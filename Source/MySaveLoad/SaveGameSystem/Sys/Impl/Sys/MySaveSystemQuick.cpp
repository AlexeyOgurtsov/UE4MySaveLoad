#include "SaveGameSystem/Sys/MySaveSystemQuick.h"
#include "SaveGameSystem/IMySaveable.h"
#include "../../MySaveableHandleObject.h"

#include "Util/Core/LogUtilLib.h"

TScriptInterface<IMySaveableHandle> UMySaveSystemQuick::CreateSaveableHandle(TScriptInterface<IMySaveable> const InSaveable) 
{
	UMySaveableHandleObject* const SaveableHandle = UMySaveableHandleObject::NewSaveableHandleObject(InSaveable, this);
	RegisterSaveableObject(InSaveable);
	return SaveableHandle;
}

void UMySaveSystemQuick::RegisterSaveableObject(TScriptInterface<IMySaveable> const InSaveable)
{
	UE_LOG(MyLog, Log, TEXT("UMySaveSystem::RegisterSaveableObject..."));
	check(InSaveable);
	UE_LOG(MyLog, Log, TEXT("%s"), *InSaveable->SaveLoad_ToStringPrefixed(TEXT("Saveable is ")));

	if(false == InSaveable.GetObject()->IsPendingKill())
	{
		bool const bAdded = SaveableObjects.AddUnique(InSaveable);
	}
	else
	{
		checkf(false, TEXT("Saveable object is marked with IsPendingKill(), and it's now treated like fatal error!"));
		UE_LOG(MyLog, Warning, TEXT("Skipping saveable object registration: IsPendingKill set!"));
	}

	UE_LOG(MyLog, Log, TEXT("UMySaveSystem::RegisterSaveableObject DONE"));
}

void UMySaveSystemQuick::NotifyObjectDestructed(TScriptInterface<IMySaveable> const InSaveable)
{
	UE_LOG(MyLog, Log, TEXT("UMySaveSystem::NotifyObjectDestructed..."));

	if(InSaveable->IsStatic())
	{
		UE_LOG(MyLog, Log, TEXT("Object is NOT dynamic, Destructed object is accounted"));
		StaticDestructedObjects.Add(InSaveable->GetUniqueFName());
	}

	UE_LOG(MyLog, Log, TEXT("UMySaveSystem::NotifyObjectDestructed DONE"));
}