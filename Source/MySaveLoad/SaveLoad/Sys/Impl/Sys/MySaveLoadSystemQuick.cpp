#include "SaveLoad/Sys/MySaveLoadSystemQuick.h"
#include "SaveLoad/IMySaveable.h"
#include "../../MySaveableHandleObject.h"

#include "Util/Core/LogUtilLib.h"

#include "Engine/World.h"

namespace 
{
	bool ShouldRegisterSaveable(TScriptInterface<IMySaveableHandle> const InSaveableHandle)
	{
		FString const PrefixString = InSaveableHandle->SaveLoad_GetPrefixString(TEXT("Skipping registration for"));
		if(false == InSaveableHandle->SaveLoad_IsEnabled())
		{
			UE_LOG(MyLog, Log, TEXT("%s SaveLoad is diabled"), *PrefixString);
			return false;
		}

		const IMySaveable* const Saveable = InSaveableHandle->SaveLoad_GetSaveable().GetInterface();
		checkf(Saveable, TEXT("%s Saveable object assigned to handle should never be nullptr"), *PrefixString);

		if(UWorld* World = InSavebleHandle->GetWorld())
		{
			warnf(World, TEXT("%s GetWorld() returned nullptr - maybe you forgot to implement GetWorld() (it's NOT implemented for UObject by default)"), *PrefixString);
			if(false == World->IsGameWorld())	
			{
				UE_LOG(MyLog, Log, TEXT("%s Object's assigned world (returned by GetWorld()) is NOT game world"), *PrefixString);
				return false;
			}
		}	

		bool const HandlePendingKill = InSaveableHandle.GetObject()->IsPendingKill();
		checkf(false == HandlePendingKill, TEXT("%s Saveable object handle is marked with IsPendingKill(), and it's now treated like fatal error!"), *PrefixString);
		bool const SaveablePendingKill = Saveable.GetObject()->IsPendingKill();
		checkf(false == SaveablePendingKill, TEXT("%s Saveable object is marked with IsPendingKill(), and it's now treated like fatal error!"), *PrefixString);	

		return true;
	}
} // anonymous

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
	UE_LOG(MyLog, Log, TEXT("%s"), *InSaveableHandle->SaveLoad_ToStringPrefixed(TEXT("Saveable is")));

	if(ShouldRegisterSaveable(InSaveableHandle))
	{
		bool const bAdded = SaveableHandles.AddUnique(InSaveableHandle);
		checkf(bAdded, TEXT("Each saveable handle must be registered only once!"));
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