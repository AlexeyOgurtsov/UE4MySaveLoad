#include "MySaverLoaderBase.h"
#include "MySaveLoadState.h"
#include "PerObjectSaveLoadData.h"

#include "SaveLoad/IMySaveableHandle.h"
#include "../Sys/IMySaveLoadSystemInternal.h"
#include "MySaveLoadSystemUtils.h"

#include "SaveLoad/Util/SaveLoadLogUtilLib.h"

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
	SL_LOGFUNC();
	checkNoRecursion();

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
}

void UMySaverLoaderBase::SerializeToFromArchive()
{
	SL_LOGFUNC_MSG(TEXT("Serializing binary world"));
	LogBinaryWorld();
	GetAr() << GetBinaryWorld();
}

void UMySaverLoaderBase::LogBinaryWorld()
{
	SL_LOG(TEXT("Binary world has:"));
	SL_LOG(TEXT("%d classes"), GetBinaryWorld().Classes.Num());
	SL_LOG(TEXT("%d global objects"), GetBinaryWorld().GlobalObjects.Num());
	SL_LOG(TEXT("%d world objects"), GetBinaryWorld().WorldObjects.Num());
}

void UMySaverLoaderBase::AssignDataToAllObjects()
{
	SL_LOGFUNC();
	for(TScriptInterface<IMySaveableHandle> SaveableHandle : GetState()->WorldSaveableHandles)
	{
		AssignObjectData(SaveableHandle);
	}
	for(TScriptInterface<IMySaveableHandle> SaveableHandle : GetState()->GlobalSaveableHandles)
	{
		AssignObjectData(SaveableHandle);
	}
}

void UMySaverLoaderBase::AssignObjectData(TScriptInterface<IMySaveableHandle> const InSaveableHandle)
{
	check(InSaveableHandle);
	auto Data = NewObject<UPerObjectSaveLoadDataBase>(GetPerObjectDataClass());
	InSaveableHandle->SaveLoad_AssignData(this, Data);
}

bool UMySaverLoaderBase::IsGlobalObject(TScriptInterface<IMySaveableHandle> const InSaveableHandle) const
{
	return State->GlobalSaveableHandles.Contains(InSaveableHandle);
}

bool UMySaverLoaderBase::ShouldObjectBeSaved(TScriptInterface<IMySaveableHandle> const InSaveableHandle, bool const bInLogged, bool const bLogOnFalseOnly) const
{
	if(InSaveableHandle == nullptr)
	{
		if(bInLogged)
		{
			SL_LOG_WARN(TEXT("Saveable handle is nullptr"));
		}
		return false;
	}

	FString const PrefixString = InSaveableHandle->SaveLoad_GetPrefixString(TEXT("Object"));

	if(bInLogged && (false == bLogOnFalseOnly) )
	{
		SL_LOG(TEXT("%s: supports IMySaveable, UniqueName is \"%s\""), *PrefixString, *InSaveableHandle->SaveLoad_GetUniqueName());
	}

	if(false == InSaveableHandle->SaveLoad_IsEnabled())
	{
		if(bInLogged)
		{
			SL_LOG(TEXT("%s: SaveLoad flag is NOT set"), *PrefixString);
		}
	
		return false;
	}

	if(bInLogged && (false == bLogOnFalseOnly) )
	{
		SL_LOG(TEXT("%s: object is to be saved"), *PrefixString);
	}

	return true;
}
