#include "MyLoaderBase.h"
#include "MySaveLoadState.h"
#include "../Sys/IMySaveLoadSystemInternal.h"
#include "MySaveLoadSystemUtils.h"
#include "SaveLoad/IMySaveable.h"
#include "SaveLoad/MySaveableUtils.h"

#include "Util/Core/LogUtilLib.h"

#include "Engine/GameInstance.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerController.h"

#include "EngineUtils.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

void UMyLoaderBase::SetupLoaderBase(IMySaveLoadSystemInternal* const InSys, FArchive* const InArchive, UWorld* const InWorld, UMySaveLoadState* const InState)
{
	UE_LOG(MyLog, Log, TEXT("SetupLoaderBase..."));

	SetupSaverLoaderBase(ESaverOrLoader::Loader, InSys, InArchive, InWorld, InState);

	UE_LOG(MyLog, Log, TEXT("SetupLoaderBase DONE"));
}

void UMyLoaderBase::Load()
{
	UE_LOG(MyLog, Log, TEXT("UMyLoaderBase::Load..."));

	CheckAndLoadBinaryFromArchive();

	{
		LoadWorld();
	}

	{
		LoadSavedClasses();
	}

	{
		DestroyExtraObjects();
		// @TODO: Loading objects
	}

	UE_LOG(MyLog, Log, TEXT("NOT YET IMPL"));

	UE_LOG(MyLog, Log, TEXT("UMyLoaderBase::Load DONE"));
}

void UMyLoaderBase::CheckAndLoadBinaryFromArchive()
{
	UE_LOG(MyLog, Log, TEXT("UMyLoaderBase::CheckAndLoadBinaryFromArchive..."));

	GetAr() << GetBinaryWorld();
	UMySaveLoadSystemUtils::CheckSavedWorldValid(&GetBinaryWorld());

	UE_LOG(MyLog, Log, TEXT("UMyLoaderBase::CheckAndLoadBinaryFromArchive DONE"));

}

void UMyLoaderBase::LoadWorld()
{
	UE_LOG(MyLog, Log, TEXT("UMyLoaderBase::LoadWorld..."));

	GetCommState()->MapName = GetBinaryWorld().WorldInfo.MapName;
	UE_LOG(MyLog, Log, TEXT("Loaded map name: \"%s\""), *GetCommState()->MapName);

	{
		UE_LOG(MyLog, Log, TEXT("Checking map name"));

		FString MapName, MapName_NoStreamingPrefix;

		{
			MapName = GetWorld()->GetMapName();
			UE_LOG(MyLog, Log, TEXT("GetWorld()->GetMapName() returned \"%s\""), *MapName);
		}

	
		{
			MapName_NoStreamingPrefix = MapName;
			MapName_NoStreamingPrefix.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

			UE_LOG(MyLog, Log, TEXT("MapName(no streaming prefix): \"%s\""), *MapName_NoStreamingPrefix);
		}

		if(MapName_NoStreamingPrefix == GetCommState()->MapName)
		{
			UE_LOG(MyLog, Log, TEXT("MapName in the save file corresponds to current level's: skipping level load!"));
		}
		else
		{
			UE_LOG(MyLog, Log, TEXT("Loading level - MapName does NOT correspond the current level's"));	
			UE_LOG(MyLog, Log, TEXT("Current map name is \"%s\" and map name in save file is \"%s\""), *MapName_NoStreamingPrefix, *GetCommState()->MapName);	
			checkf(false, TEXT("Not yet impl: here we must load the level"));
		}

		UE_LOG(MyLog, Log, TEXT("Checking map name DONE"));
	}

	UE_LOG(MyLog, Log, TEXT("UMyLoaderBase::LoadWorld DONE"));
}

void UMyLoaderBase::LoadSavedClasses()
{
	UE_LOG(MyLog, Log, TEXT("UMyLoaderBase::LoadSavedClasses..."));

	UE_LOG(MyLog, Log, TEXT("%d classes in the save file"), GetBinaryWorld().Classes.Num());
	GetCommState()->Classes.SetNum(GetBinaryWorld().Classes.Num());
	for(int32 Index = 0; Index < GetBinaryWorld().Classes.Num(); Index++)
	{
		const FMySavedClass* SavedClass = &GetBinaryWorld().Classes[Index];
		LoadSavedClass(Index, &GetCommState()->Classes[Index], SavedClass);
	}

	UE_LOG(MyLog, Log, TEXT("UMyLoaderBase::LoadSavedClasses DONE"));
}

void UMyLoaderBase::LoadSavedClass(int32 InClassIndex, UClass**ppOutClass, const FMySavedClass* SavedClass)
{
	UE_LOG(MyLog, Log, TEXT("Loading saved class named \"%s\" (index=%d)..."), *SavedClass->Name, InClassIndex);
	
	check(ppOutClass);
	check(SavedClass);

	*ppOutClass = FindObject<UClass>(ANY_PACKAGE, *SavedClass->Name);
	checkf(*ppOutClass, TEXT("FindObject<UClass> must succeed!!!"));

	UE_LOG(MyLog, Log, TEXT("Loading saved class DONE"));
}

void UMyLoaderBase::DestroyExtraObjects()
{
	UE_LOG(MyLog, Log, TEXT("Destrong extra objects..."));
	UE_LOG(MyLog, Log, TEXT("%d objects are marked as destroyed in the save file"), GetBinaryWorld().StaticDestructedObjects.Num());
	GetCommState()->StaticDestructedObjects.Reserve(GetBinaryWorld().StaticDestructedObjects.Num());
	for(const FMySavedDestruct& SavedObjectToDestruct: GetBinaryWorld().StaticDestructedObjects)
	{
		GetCommState()->StaticDestructedObjects.Add(FName(*SavedObjectToDestruct.UniqueName));

		// @TODO: Find object by name
		UE_LOG(MyLog, Error, TEXT("not yet impl")); TScriptInterface<IMySaveable> Obj = nullptr;

		if(Obj)
		{
			TArray<FStringFormatArg> FormatArgs;
			FormatArgs.Add(Obj->SaveLoad_GetUniqueName());
			FormatArgs.Add(Obj.GetObject()->GetClass()->GetName());
			FString PrefixString = FString::Format(TEXT("Destring extra object: checking object with UniqueName \"{0}\" of class \"{1}\": "), FormatArgs);

			UE_LOG(MyLog, Log, TEXT("%sDestroying object..."), *PrefixString);

			// Chance for object to do some work before destruction		
			Obj->SaveLoad_BeforeDestroy();

			// @TODO: Is it right? Maybe we have some links to object still (in the renderer maybe)?
			UE_LOG(MyLog, Error, TEXT("not yet impl: destruction")); //Obj->BeginDestroy();
		}
	}
	UE_LOG(MyLog, Log, TEXT("Destrong extra objects DONE"));
}

TScriptInterface<IMySaveable> UMyLoaderBase::LoadSavedObject(const FMySavedObject* const pSavedObject)
{
	UE_LOG(MyLog, Error, TEXT("Not yet impl")); return nullptr;
}
      