#include "SaveLoad/Sys/MySaveLoadSystemQuick.h"
#include "SaveLoad/IMySaveable.h"
#include "../../MySaveableHandleObject.h"

// @TODO: Include saver and loader here
//#include "Sys/Impl/SaverLoader/MyLoaderBase.h"
//#include "Sys/Impl/SaverLoader/MySaverBase.h"

#include "SaveLoad/Util/SaveLoadLogUtilLib.h"

#include "Engine/World.h"

UMySaveLoadSystemQuick* UMySaveLoadSystemQuick::NewQuickSaveLoadSystem(UObject* InOuter, const FQuickSaveLoadSystemInitializer& InInitializer)
{
	UMySaveLoadSystemQuick* const Obj = NewObject<UMySaveLoadSystemQuick>(InOuter);
	Obj->InitializeQuickSaveLoad(InOuter, InInitializer);
	return Obj;
}

UMySaveLoadSystemQuick* UMySaveLoadSystemQuick::CreateDefaultQuickSaveLoadSystem(UObject* InOuter, const FQuickSaveLoadSystemInitializer& InInitializer)
{
	check(InOuter);
	UMySaveLoadSystemQuick* const Obj = InOuter->CreateDefaultSubobject<UMySaveLoadSystemQuick>(TEXT("QuickSaveLoadSystem"));
	Obj->InitializeQuickSaveLoad(InOuter, InInitializer);
	return Obj;
}

void UMySaveLoadSystemQuick::InitializeQuickSaveLoad(UObject* InOuter, const FQuickSaveLoadSystemInitializer& InInitializer)
{
	SL_EMPTY();
}

void UMySaveLoadSystemQuick::Load(FArchive& Ar)
{
	SL_NOT_IMPL();
}

void UMySaveLoadSystemQuick::Save(FArchive& Ar)
{
	SL_NOT_IMPL();
}

namespace 
{
	bool ShouldRegisterSaveable(TScriptInterface<IMySaveableHandle> const InSaveableHandle)
	{
		checkNoRecursion();

		FString const PrefixString = InSaveableHandle->SaveLoad_GetPrefixString(TEXT("Skipping registration for"));
		if(false == InSaveableHandle->SaveLoad_IsEnabled())
		{
			SL_LOG(TEXT("%s SaveLoad is diabled"), *PrefixString);
			return false;
		}

		const IMySaveable* const Saveable = Cast<IMySaveable>(InSaveableHandle->SaveLoad_GetSaveable().GetObject());
		UObject* const SaveableObject = InSaveableHandle->SaveLoad_GetSaveable().GetObject();
		SL_LOG_ERROR(TEXT("%s Saveable object assigned to handle should never be nullptr"), *PrefixString);

		if(const UWorld* const World = InSaveableHandle.GetObject()->GetWorld())
		{
			SL_LOG_WARN(TEXT("%s GetWorld() returned nullptr - maybe you forgot to implement GetWorld() (it's NOT implemented for UObject by default)"), *PrefixString);
			if(false == World->IsGameWorld())	
			{
				SL_LOG(TEXT("%s Object's assigned world (returned by GetWorld()) is NOT game world"), *PrefixString);
				return false;
			}
		}	

		bool const HandlePendingKill = InSaveableHandle.GetObject()->IsPendingKill();
		checkf(false == HandlePendingKill, TEXT("%s Saveable object handle is marked with IsPendingKill(), and it's now treated like fatal error!"), *PrefixString);
		bool const SaveablePendingKill = SaveableObject->IsPendingKill();
		checkf(false == SaveablePendingKill, TEXT("%s Saveable object is marked with IsPendingKill(), and it's now treated like fatal error!"), *PrefixString);	

		return true;
	}
} // anonymous

void UMySaveLoadSystemQuick::RegisterSaveableObject(TScriptInterface<IMySaveableHandle> const InSaveableHandle)
{
	SL_LOGFUNC_MSG(TEXT("Registering saveable object"));
	checkNoRecursion();

	check(InSaveableHandle);
	SL_LOG(TEXT("%s"), *InSaveableHandle->SaveLoad_ToStringPrefixed(TEXT("Saveable is")));

	if(ShouldRegisterSaveable(InSaveableHandle))
	{
		bool const bAdded = (1 == SaveableHandles.AddUnique(InSaveableHandle));
		checkf(bAdded, TEXT("Each saveable handle must be registered only once!"));
	}
}

void UMySaveLoadSystemQuick::UnregisterSaveableObjectChecked(TScriptInterface<IMySaveableHandle> const InSaveableHandle)
{
	checkNoRecursion();

	check(InSaveableHandle);
	int32 bFoundAndRemoved = (1 == SaveableHandles.RemoveSingleSwap(InSaveableHandle));
	checkf(bFoundAndRemoved, TEXT("The given saveable handle must be registered"));
}

TScriptInterface<IMySaveableHandle> UMySaveLoadSystemQuick::CreateSaveableHandle_Implementation(const TScriptInterface<IMySaveable>& InSaveable)
{
	checkNoRecursion();

	UMySaveableHandleObject* const SaveableHandle = UMySaveableHandleObject::CreateSaveableHandleDefaultSubobject(InSaveable, this);
	RegisterSaveableObject(SaveableHandle);
	SL_RETURN_CHECK_MSG(SaveableHandle, SaveableHandle, TEXT("Returned saveable handle must be valid NON-null pointer"));
}

void UMySaveLoadSystemQuick::NotifySaveableDestructed_Implementation(const TScriptInterface<IMySaveableHandle>& InSaveableHandle)
{
	SL_LOGFUNC_MSG(TEXT("Notified that object is destructed"));
	checkNoRecursion();

	if(InSaveableHandle->SaveLoad_IsEnabled())
	{
		if(InSaveableHandle->SaveLoad_IsStatic())
		{
			UnregisterSaveableObjectChecked(InSaveableHandle);

			SL_LOG(TEXT("Object is static, destructed object is accounted"));
			StaticDestructedObjects.Add(InSaveableHandle->SaveLoad_GetUniqueFName());
		}
	}
	else
	{
		SL_LOG(TEXT("Object: SaveLoad is NOT enabled - skipping accounting"));
	}
}
