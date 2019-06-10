#include "MySaverBase.h"
#include "MySaveLoadState.h"
#include "MySaveLoadSystemUtils.h"
#include "SaveLoad/IMySaveable.h"
#include "SaveLoad/IMySaveableHandle.h"
#include "SaveLoad/MySaveableUtils.h"
#include "PerObjectSaveLoadData.h"
#include "../Sys/IMySaveLoadSystemInternal.h"

#include "SaveLoad/Util/SaveLoadLogUtilLib.h"

#include "Engine/GameInstance.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerController.h"

#include "EngineUtils.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

void UMySaverBase::SetupSaverBase(IMySaveLoadSystemInternal* const InSys, FArchive* const InArchive, UWorld* const InWorld, UMySaveLoadState* const InState)
{
	SL_LOGFUNC();
	checkNoRecursion();

	SetupSaverLoaderBase(ESaverOrLoader::Saver, InSys, InArchive, InWorld, InState);
}

void UMySaverBase::Save()
{
	SL_LOGFUNC();
	checkNoRecursion();

	{
		 Extract_WorldInfo();
	}

	{
		Extract_DestructedObjects();
	}

	{
		Find_GlobalObjects();
		Find_WorldObjects();
		AssignDataToAllObjects();
		Extract_ClassTable();
	}

	BinarizeToArchive();
}

void UMySaverBase::Extract_WorldInfo()
{
	SL_LOGFUNC_MSG(TEXT("Extracing info about the world"));

	{
		SL_LOGBLOCK(TEXT("Extracing map name"));

		FString MapName, MapName_NoStreamingPrefix;

		{
			MapName = GetWorld()->GetMapName();
			SL_LOG(TEXT("GetWorld()->GetMapName() returned \"%s\""), *MapName);
		}

	
		{
			MapName_NoStreamingPrefix = MapName;
			MapName_NoStreamingPrefix.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

			GetState()->MapName = MapName_NoStreamingPrefix;

			SL_LOG(TEXT("MapName(no streaming prefix): \"%s\""), *MapName_NoStreamingPrefix);
		}

	}

	UE_LOG(MyLog, Log, TEXT("Extracing info about the world DONE"));
}

void UMySaverBase::Extract_DestructedObjects()
{
	SL_LOGFUNC_MSG(TEXT("Extracing info about destructed objects"));
	GetState()->StaticDestructedObjects = GetSys()->GetStaticDestructedObjects();
	SL_LOG(TEXT("%d destructed objects found"), GetState()->StaticDestructedObjects.Num());
	for(const FName& DestructObjectName : GetState()->StaticDestructedObjects)
	{
		SL_LOG(TEXT("DestructedObject: named \"%s\""), *DestructObjectName.ToString());
	}
}

void UMySaverBase::BinarizeToArchive()
{
	SL_LOGFUNC_MSG(TEXT("Binarizing to archive..."));
	UMySaveLoadSystemUtils::SaveLoadStateToSaveStruct(this, &GetBinaryWorld(), GetState());
	SerializeToFromArchive();
}

void UMySaverBase::Find_WorldObjects()
{
	SL_LOGFUNC_MSG(TEXT("Searching world objects"))

	int32 NumGlobalObjectsInWorld = 0;
	for(TArray<TScriptInterface<IMySaveableHandle>>::TConstIterator Itr = GetSys()->CreateSaveableHandleIterator(); Itr; ++Itr)
	{
		if(ShouldObjectBeSaved(*Itr, ELogFlags::None))
		{
			if( false == IsGlobalObject(*Itr) )	
			{
				GetState()->WorldSaveableHandles.AddUnique(*Itr);
			}
			else
			{
				SL_LOG(TEXT("Global object \"%s\" found"), *(*Itr)->SaveLoad_ToString());
				NumGlobalObjectsInWorld++;
			}
		}		
	}
	UE_LOG(MyLog, Log, TEXT("%d saveable actors found, %d global objects in world"), GetState()->WorldSaveableHandles.Num(), NumGlobalObjectsInWorld);
}

void UMySaverBase::Find_GlobalObjects()
{
	SL_LOGFUNC_MSG(TEXT("Searching global objects"));

	SL_IMPL_NOTE(TEXT("Should we at all search global objects?"));
	Find_GlobalObject_PlayerController();
	Find_GlobalObject_GameMode();
	Find_GlobalObject_GameInstance();

	SL_LOG(TEXT("Total %d global objects registered"), GetState()->GlobalSaveableHandles.Num());
}


void UMySaverBase::Find_GlobalObject_PlayerController()
{
	SL_LOGFUNC_MSG(TEXT("Searching player controller"));
	
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if(PC == nullptr)
	{
		SL_LOG_WARN(TEXT("UGameplayStatics::GetPlayerController() returned nullptr"));
	}
	else
	{
		RegisterGlobalObject_IfShouldBeSaved(PC);
	}
}

void UMySaverBase::Find_GlobalObject_GameMode()
{
	SL_LOGFUNC_MSG(TEXT("Searching game mode"));
	
	AGameModeBase* G = GetWorld()->GetAuthGameMode();
	if(G == nullptr)
	{
		SL_LOG_WARN(TEXT("GetAuthGameMode() returned nullptr!"));
	}
	else
	{
		RegisterGlobalObject_IfShouldBeSaved(G);
	}
}

void UMySaverBase::Find_GlobalObject_GameInstance()
{
	SL_LOGFUNC_MSG(TEXT("Searching Game Instance"));

	UGameInstance* const G = GetWorld()->GetGameInstance();
	RegisterGlobalObject_IfShouldBeSaved(G);

	UE_LOG(MyLog, Log, TEXT("Find_GlobalObject_GameInstance DONE"));
}

void UMySaverBase::Extract_ClassTable()
{
	SL_LOGFUNC_MSG(TEXT("Extracing class table"));
	
	for(TScriptInterface<IMySaveableHandle> SaveableHandle : GetState()->GlobalSaveableHandles)
	{
		GetState()->Classes.AddUnique(SaveableHandle->SaveLoad_GetSaveable().GetObject()->GetClass());
	}

	for(TScriptInterface<IMySaveableHandle> SaveableHandle : GetState()->WorldSaveableHandles)
	{
		GetState()->Classes.AddUnique(SaveableHandle->SaveLoad_GetSaveable().GetObject()->GetClass());
	}

	UE_LOG(MyLog, Log, TEXT("Total %d classes extracted"), GetState()->Classes.Num());

	BindClassIndicesToObjects(GetState()->GlobalSaveableHandles);
	BindClassIndicesToObjects(GetState()->WorldSaveableHandles);
}

void UMySaverBase::BindClassIndicesToObjects(const TArray<TScriptInterface<IMySaveableHandle>>& InSaveableHandles)
{
	for(TScriptInterface<IMySaveableHandle> SaveableHandle : InSaveableHandles)
	{
		int32 ClassIndex;
		UClass* TheClass = SaveableHandle->SaveLoad_GetSaveable().GetObject()->GetClass();
		FString SaveableString = SaveableHandle->SaveLoad_ToString(); 

		bool const bClassFound = GetState()->Classes.Find(TheClass, /*Out*/ClassIndex);
		checkf(bClassFound, TEXT("Class for object \"%s\" must be registered in the class table"), *SaveableString);
		UPerObjectSaveLoadDataBase* const ObjDataBase = SaveableHandle->SaveLoad_GetData(this);
		checkf(bClassFound, TEXT("Class for object \"%s\": Saveable data object must be assigned!"), *SaveableString);
		auto ObjData = CastChecked<UPerObjectSaveLoadData>(ObjDataBase);
		ObjData->ClassIndex = ClassIndex;
		
	}
}

void UMySaverBase::RegisterGlobalObject(TScriptInterface<IMySaveableHandle> const InSaveableHandle)
{
	SL_LOGFUNC_MSG(TEXT("Registering global object"));
	check(InSaveableHandle);
	SL_LOG(TEXT("Object \"%s\""), *InSaveableHandle->SaveLoad_ToString());
	bool const bAdded = (1 == GetState()->GlobalSaveableHandles.AddUnique(InSaveableHandle));
	checkf(bAdded, TEXT("Saveable handle must be successfully added to the container"));
}

void UMySaverBase::RegisterGlobalObject_IfShouldBeSaved(UObject* const InObject)
{
	SL_LOGFUNC();
	checkNoRecursion();

	if(InObject == nullptr)
	{
		SL_LOG_WARN(TEXT("Skipping object registration - passed object is nullptr"));
		return;
	}

	if(IMySaveable* Saveable = Cast<IMySaveable>(InObject))
	{
		TScriptInterface<IMySaveableHandle> const SaveableHandle = Saveable->SaveLoad_GetHandle();
		checkf(SaveableHandle, TEXT("SaveableHandle should never be nullptr"));
		SL_LOG(TEXT("Object \"%s\""), *SaveableHandle->SaveLoad_ToString());

		if(ShouldObjectBeSaved(SaveableHandle, ELogFlags::None))
		{
			RegisterGlobalObject(SaveableHandle);
		}
	}
	else
	{
		checkf(false, TEXT("Skipping registration - saveable interface is not supported"));
	}
}