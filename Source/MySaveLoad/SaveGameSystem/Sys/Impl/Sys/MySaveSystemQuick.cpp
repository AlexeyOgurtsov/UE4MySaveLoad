#include "SaveGameSystem/Sys/MySaveSystemQuick.h"
#include "SaveGameSystem/IMySaveable.h"
#include "Util/Core/LogUtilLib.h"

void UMySaveSystemQuick::RegisterSaveableObject(TScriptInterface<IMySaveable> const InSaveable)
{
	UE_LOG(MyLog, Log, TEXT("UMySaveSystem::RegisterSaveableObject..."));
	check(InSaveable);
	UE_LOG(MyLog, Log, TEXT("Object named \"%s\" of class \"%s\""), *InSaveable.GetObject()->GetName(), *InSaveable.GetObject()->GetClass()->GetName());

	if(false == InSaveable.GetObject()->IsPendingKill())
	{
		bool const bAdded = SaveableObjects.AddUnique(InSaveable);
	}
	else
	{
		UE_LOG(MyLog, Warning, TEXT("Skipping saveable object registration: IsPendingKill set!"));
	}

	UE_LOG(MyLog, Log, TEXT("UMySaveSystem::RegisterSaveableObject DONE"));
}

void UMySaveSystemQuick::NotifyObjectDestructed(TScriptInterface<IMySaveable> const InObject)
{
	UE_LOG(MyLog, Log, TEXT("UMySaveSystem::NotifyObjectDestructed..."));

	if(InObject->IsStatic())
	{
		UE_LOG(MyLog, Log, TEXT("Object is NOT dynamic, Destructed object is accounted"));
		StaticDestructedObjects.Add(InObject->GetUniqueFName());
	}

	UE_LOG(MyLog, Log, TEXT("UMySaveSystem::NotifyObjectDestructed DONE"));
}