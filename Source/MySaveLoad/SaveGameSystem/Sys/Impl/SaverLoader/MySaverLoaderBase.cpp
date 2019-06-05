#include "MySaverLoaderBase.h"
#include "MySaveLoadState.h"
#include "PerObjectSaveLoadData.h"

#include "SaveGameSystem/IMySaveable.h"
#include "SaveGameSystem/MySaveableUtils.h"
#include "../Sys/IMySaveSystemInternal.h"
#include "MySaveLoadSystemUtils.h"

#include "Util/Core/LogUtilLib.h"

#include "Engine/World.h"
#include "Serialization/Archive.h"


void UMySaverLoaderBase::SetupSaverLoaderBase
(
	ESaverOrLoader InSaverOrLoader, 
	IMySaveSystemInternal* InSys,
	FArchive* InArchive, UWorld* InWorld,
	UMySaveLoadState* InCommState
)
{
	UE_LOG(MyLog, Log, TEXT("SetupSaverLoaderBase..."));

	check(InSys);
	check(InArchive);
	check(InWorld);
	checkf(InWorld->IsGameWorld(), TEXT("UMySaverLoaderBase: world MUST be Game World!"));
	check(InCommState);

	SaverOrLoader = InSaverOrLoader;
	Sys = InSys;
	Ar = InArchive;
	World = InWorld;
	CommState = InCommState;

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
	for(TScriptInterface<IMySaveable> O : GetCommState()->WorldObjects)
	{
		AssignObjectData(O);
	}
	for(TScriptInterface<IMySaveable> O : GetCommState()->GlobalObjects)
	{
		AssignObjectData(O);
	}
	UE_LOG(MyLog, Log, TEXT("UMySaverLoaderBase::AssignDataToAllObjects DONE"));
}

void UMySaverLoaderBase::AssignObjectData(TScriptInterface<IMySaveable> InObj)
{
	check(InObj);
	auto Data = NewObject<UPerObjectSaveLoadDataBase>(GetPerObjectDataClass());
	InObj->SaveLoad_AssignData(this, Data);
}

bool UMySaverLoaderBase::IsGlobalObject(UObject* InObject) const
{
	return CommState->GlobalObjects.Contains(TScriptInterface<IMySaveable>(InObject));
}

bool UMySaverLoaderBase::ShouldObjectBeSaved(UObject* InObj, bool bInLogged, bool bLogOnFalseOnly) const
{
	if(InObj == nullptr)
	{
		if(bInLogged)
		{
			UE_LOG(MyLog, Warning, TEXT("Object is nullptr"));
		}
		return false;
	}

	TArray<FStringFormatArg> FormatArgs;
	FormatArgs.Add(*InObj->GetName());
	FormatArgs.Add(*InObj->GetClass()->GetName());
	FString PrefixString = FString::Format(TEXT("Object \"%s\" of class \"%s\""), FormatArgs);

	if(IMySaveable* SaveableObj = Cast<IMySaveable>(InObj))
	{
		FName UniqueName = UMySaveableUtils::GetUniqueName(InObj);
		if(bInLogged && (false == bLogOnFalseOnly) )
		{
			UE_LOG(MyLog, Log, TEXT("%s: supports IMySaveable, UniqueName is \"%s\""), *PrefixString, *UniqueName.ToString());
		}

		if(false == UMySaveableUtils::IsSaveLoad(InObj))
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
	}
	else
	{
		if(bInLogged)
		{
			UE_LOG(MyLog, Warning, TEXT("%s: object NOT instance of IMySaveable"), *PrefixString);
		}
	}

	return true;
}
