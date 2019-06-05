#include "SaveGameSystem/Sys/MySaveSystemQuick.h"
#include "SaveGameSystem/IMySaveable.h"
#include "Util/Core/LogUtilLib.h"

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