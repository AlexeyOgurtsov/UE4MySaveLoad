#include "MySaverLoaderBase.h"
#include "MySaveLoadState.h"
#include "PerObjectSaveLoadData.h"

#include "SaveLoad/IMySaveable.h"
#include "SaveLoad/IMySaveableHandle.h"
#include "../Sys/IMySaveLoadSystemInternal.h"
#include "MySaveLoadSystemUtils.h"

#include "Util/Core/LogUtilLib.h"

#include "Engine/World.h"
#include "Serialization/Archive.h"


void UMySaverLoaderBase::SetupSaverLoaderBase
(
	ESaverOrLoader const InSaverOrLoader, 
	IMySaveLoadSystemInternal* const InSys,
	FArchive* const InArchive, UWorld* const InWorld,
	UMySaveLoadState* const InState
)
{
	UE_LOG(MyLog, Log, TEXT("SetupSaverLoaderBase..."));

	check(InSys);
	check(InArchive);
	check(InWorld);
	checkf(InWorld->IsGameWorld(), TEXT("UMySaverLoaderBase: world MUST be Game World!"));
	check(InState);

	SaverOrLoader = InSaverOrLoader;
	Sys = InSys;
	Ar = InArchive;
	World = InWorld;
	State = InState;

	PerObjectDataClass = UPerObjectSaveLoadData::StaticClass();

	UE_LOG(MyLog, Log, TEXT("SetupSaverLoaderBase DONE"));
}

void UMySaverLoaderBase::SerializeToFromArchive()
{
	LogBinaryWorld();

	UE_LOG(MyLog, Log, TEXT("Serializing binary world..."));
	GetAr() << GetBinaryWorld();
	UE_LOG(MyLog, Log, TEXT("Serializing binary world DONE"));
}

void UMySaverLoaderBase::LogBinaryWorld()
{
	UE_LOG(MyLog, Log, TEXT("Binary world has:"));

	UE_LOG(MyLog, Log, TEXT("%d classes"), GetBinaryWorld().Classes.Num());
	UE_LOG(MyLog, Log, TEXT("%d global objects"), GetBinaryWorld().GlobalObjects.Num());
	UE_LOG(MyLog, Log, TEXT("%d world objects"), GetBinaryWorld().WorldObjects.Num());
}

void UMySaverLoaderBase::AssignDataToAllObjects()
{
	UE_LOG(MyLog, Log, TEXT("UMySaverLoaderBase::AssignDataToAllObjects..."));
	for(TScriptInterface<IMySaveable> O : GetState()->WorldObjects)
	{
		AssignObjectData(O);
	}
	for(TScriptInterface<IMySaveable> O : GetState()->GlobalObjects)
	{
		AssignObjectData(O);
	}
	UE_LOG(MyLog, Log, TEXT("UMySaverLoaderBase::AssignDataToAllObjects DONE"));
}

void UMySaverLoaderBase::AssignObjectData(TScriptInterface<IMySaveable> const InObj)
{
	check(InObj);
	auto Data = NewObject<UPerObjectSaveLoadDataBase>(GetPerObjectDataClass());
	InObj->SaveLoad_GetHandle()->SaveLoad_AssignData(this, Data);
}

bool UMySaverLoaderBase::IsGlobalObject(TScriptInterface<IMySaveableHandle> const InSaveableHandle) const
{
	return State->GlobalObjects.Contains(InSaveableHandle->SaveLoad_GetSaveable());
}

bool UMySaverLoaderBase::ShouldObjectBeSaved(TScriptInterface<IMySaveableHandle> const InSaveableHandle, bool const bInLogged, bool const bLogOnFalseOnly) const
{
	if(InSaveableHandle == nullptr)
	{
		if(bInLogged)
		{
			UE_LOG(MyLog, Warning, TEXT("Saveable handle is nullptr"));
		}
		return false;
	}

	FString const PrefixString = InSaveableHandle->SaveLoad_GetPrefixString(TEXT("Object"));

	if(bInLogged && (false == bLogOnFalseOnly) )
	{
		UE_LOG(MyLog, Log, TEXT("%s: supports IMySaveable, UniqueName is \"%s\""), *PrefixString, *InSaveableHandle->SaveLoad_GetUniqueName());
	}

	if(false == InSaveableHandle->SaveLoad_IsEnabled())
	{
		if(bInLogged)
		{
			UE_LOG(MyLog, Log, TEXT("%s: SaveLoad flag is NOT set"), *PrefixString);
		}
	
		return false;
	}

	if(bInLogged && (false == bLogOnFalseOnly) )
	{
		UE_LOG(MyLog, Log, TEXT("%s: object is to be saved"), *PrefixString);
	}

	return true;
}
