#include "SaveGameSystem/Sys/MySaveSystemQuick.h"
#include "SaveGameSystem/IMySaveable.h"
#include "SaveGameSystem/MySaveableUtils.h"
#include "Util/Core/LogUtilLib.h"

void UMySaveSystemQuick::NotifyObjectDestructed(TScriptInterface<IMySaveable> InObject)
{
	UE_LOG(MyLog, Log, TEXT("UMySaveSystem::NotifyObjectDestructed..."));

	if(UMySaveableUtils::IsStatic(InObject))
	{
		UE_LOG(MyLog, Log, TEXT("Object is NOT dynamic, Destructed object is accounted"));
		DestructedObjects.Add(UMySaveableUtils::GetUniqueName(InObject));
	}

	UE_LOG(MyLog, Log, TEXT("UMySaveSystem::NotifyObjectDestructed DONE"));
}