#include "SaveLoad/Sys/MySaveLoadSystemQuick.h"
#include "SaveLoad/IMySaveable.h"
#include "../../MySaveableHandleObject.h"

#include "Util/Core/LogUtilLib.h"

TScriptInterface<IMySaveableHandle> UMySaveLoadSystemQuick::CreateSaveableHandle(TScriptInterface<IMySaveable> const InSaveable) 
{
	UMySaveableHandleObject* const SaveableHandle = UMySaveableHandleObject::CreateSaveableHandleDefaultSubobject(InSaveable, this);
	RegisterSaveableObject(SaveableHandle);
	return SaveableHandle;
}

void UMySaveLoadSystemQuick::RegisterSaveableObject(TScriptInterface<IMySaveableHandle> const InSaveableHandle)
{
	UE_LOG(MyLog, Log, TEXT("UMySaveLoadSystem::RegisterSaveableObject..."));
	check(InSaveableHandle);
	UE_LOG(MyLog, Log, TEXT("%s"), *InSaveableHandle->SaveLoad_ToStringPrefixed(TEXT("Saveable is ")));

	if(InSaveableHandle->SaveLoad_IsEnabled())
	{
		TScriptInterface<IMySaveable> const Saveable = InSaveableHandle->SaveLoad_GetSaveable();
		bool const HandlePendingKill = InSaveableHandle.GetObject()->IsPendingKill();
		checkf(false == HandlePendingKill, TEXT("Saveable object handle is marked with IsPendingKill(), and it's now treated like fatal error!"));
		bool const SaveablePendingKill = Saveable.GetObject()->IsPendingKill();
		checkf(false == SaveablePendingKill, TEXT("Saveable object is marked with IsPendingKill(), and it's now treated like fatal error!"));
		if(false == HandlePendingKill && false == SaveablePendingKill)
		{
			bool const bAdded = SaveableHandles.AddUnique(InSaveableHandle);
		}
		else
		{
			UE_LOG(MyLog, Warning, TEXT("Skipping saveable object registration: IsPendingKill set on handle or object!"));
		}
	}

	UE_LOG(MyLog, Log, TEXT("UMySaveLoadSystem::RegisterSaveableObject DONE"));
}

void UMySaveLoadSystemQuick::UnregisterSaveableObjectChecked(TScriptInterface<IMySaveableHandle> const InSaveableHandle)
{
	check(InSaveableHandle);
	int32 bFoundAndRemoved = (1 == SaveableHandles.RemoveSingleSwap(InSaveableHandle));
	checkf(bFoundAndRemoved, TEXT("The given saveable handle must be registered"));
}

void UMySaveLoadSystemQuick::NotifyObjectDestructed(TScriptInterface<IMySaveableHandle> const InSaveableHandle)
{
	UE_LOG(MyLog, Log, TEXT("UMySaveLoadSystem::NotifyObjectDestructed..."));

	if(InSaveableHandle->SaveLoad_IsEnabled())
	{
		if(InSaveableHandle->SaveLoad_IsStatic())
		{
			UnregisterSaveableObjectChecked(InSaveableHandle);

			UE_LOG(MyLog, Log, TEXT("Object is static, destructed object is accounted"));
			StaticDestructedObjects.Add(InSaveableHandle->SaveLoad_GetUniqueFName());
		}
	}
	else
	{
		UE_LOG(MyLog, Log, TEXT("Object: SaveLoad is NOT enabled - skipping accounting"));
	}

	UE_LOG(MyLog, Log, TEXT("UMySaveLoadSystem::NotifyObjectDestructed DONE"));
}