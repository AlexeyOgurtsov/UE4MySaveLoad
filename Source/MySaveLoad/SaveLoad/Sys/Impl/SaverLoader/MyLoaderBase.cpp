#include "MyLoaderBase.h"
#include "MySaveLoadState.h"
#include "../Sys/IMySaveLoadSystemInternal.h"
#include "MySaveLoadSystemUtils.h"
#include "SaveLoad/IMySaveable.h"
#include "SaveLoad/MySaveableUtils.h"

#include "SaveLoad/Util/SaveLoadLogUtilLib.h"

#include "Engine/GameInstance.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerController.h"

#include "EngineUtils.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

void UMyLoaderBase::SetupLoaderBase(IMySaveLoadSystemInternal* const InSys, FArchive* const InArchive, UWorld* const InWorld, UMySaveLoadState* const InState)
{
	SL_LOGFUNC();
	checkNoRecursion();
	SetupSaverLoaderBase(ESaverOrLoader::Loader, InSys, InArchive, InWorld, InState);
}

void UMyLoaderBase::Load()
{
	SL_LOGFUNC();
	checkNoRecursion();

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

	SL_NOT_IMPL(TEXT("Function is not complete yet"));
}

void UMyLoaderBase::CheckAndLoadBinaryFromArchive()
{
	SL_LOGFUNC();
	checkNoRecursion();

	GetAr() << GetBinaryWorld();
	UMySaveLoadSystemUtils::CheckSavedWorldValid(&GetBinaryWorld());
}

void UMyLoaderBase::LoadWorld()
{
	SL_LOGFUNC();
	checkNoRecursion();

	GetState()->MapName = GetBinaryWorld().WorldInfo.MapName;
	SL_LOG(TEXT("Loaded map name: \"%s\""), *GetState()->MapName);

	{
		SL_LOGBLOCK(TEXT("Checking map name"));

		FString MapName, MapName_NoStreamingPrefix;

		{
			MapName = GetWorld()->GetMapName();
			SL_LOG(TEXT("GetWorld()->GetMapName() returned \"%s\""), *MapName);
		}

	
		{
			MapName_NoStreamingPrefix = MapName;
			MapName_NoStreamingPrefix.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

			SL_LOG(TEXT("MapName(no streaming prefix): \"%s\""), *MapName_NoStreamingPrefix);
		}

		if(MapName_NoStreamingPrefix == GetState()->MapName)
		{
			SL_LOG(TEXT("MapName in the save file corresponds to current level's: skipping level load!"));
		}
		else
		{
			SL_LOG(TEXT("Loading level - MapName does NOT correspond the current level's"));	
			SL_LOG(TEXT("Current map name is \"%s\" and map name in save file is \"%s\""), *MapName_NoStreamingPrefix, *GetState()->MapName);	
			SL_TO_BE_IMPL(TEXT("here we must load the level"));
		}
	}
}

void UMyLoaderBase::LoadSavedClasses()
{
	SL_LOGFUNC();
	checkNoRecursion();

	SL_LOG(TEXT("%d classes in the save file"), GetBinaryWorld().Classes.Num());
	GetState()->Classes.SetNum(GetBinaryWorld().Classes.Num());
	for(int32 Index = 0; Index < GetBinaryWorld().Classes.Num(); Index++)
	{
		const FMySavedClass* SavedClass = &GetBinaryWorld().Classes[Index];
		LoadSavedClass(Index, &GetState()->Classes[Index], SavedClass);
	}
}

void UMyLoaderBase::LoadSavedClass(int32 InClassIndex, UClass**ppOutClass, const FMySavedClass* SavedClass)
{
	SL_LOGFUNC_MSG(TEXT("Loading saved class named \"%s\" (index=%d)..."), *SavedClass->Name, InClassIndex);
	checkNoRecursion();
	
	check(ppOutClass);
	check(SavedClass);

	*ppOutClass = FindObject<UClass>(ANY_PACKAGE, *SavedClass->Name);
	checkf(*ppOutClass, TEXT("FindObject<UClass> must succeed!!!"));
}

void UMyLoaderBase::DestroyExtraObjects()
{
	SL_LOGFUNC_MSG(TEXT("Destroying extra objects"));
	checkNoRecursion();

	SL_LOG(TEXT("%d objects are marked as destroyed in the save file"), GetBinaryWorld().StaticDestructedObjects.Num());
	GetState()->StaticDestructedObjects.Reserve(GetBinaryWorld().StaticDestructedObjects.Num());
	for(const FMySavedDestruct& SavedObjectToDestruct: GetBinaryWorld().StaticDestructedObjects)
	{
		GetState()->StaticDestructedObjects.Add(FName(*SavedObjectToDestruct.UniqueName));

		// @TODO: Find object by name
		SL_NOT_IMPL(TEXT("Find object by name")); TScriptInterface<IMySaveable> Obj = nullptr;

		if(Obj)
		{
			FString const PrefixString = FString::Printf(TEXT("Destring extra object: \"%s\""), *Obj->SaveLoad_ToString());

			SL_LOG(TEXT("%sDestroying object..."), *PrefixString);

			// Chance for object to do some work before destruction		
			Obj->SaveLoad_BeforeDestroy();

			{
				SL_NOT_IMPL(TEXT("Destruction"));
				// @TODO: Is it right? Maybe we have some links to object still (in the renderer maybe)?
			}
		}
	}
}

TScriptInterface<IMySaveableHandle> UMyLoaderBase::LoadSavedObject(const FMySavedObject* const pSavedObject)
{
	SL_NOT_IMPL_RET(TScriptInterface<IMySaveableHandle>(), TEXT("Write it"));
}
      