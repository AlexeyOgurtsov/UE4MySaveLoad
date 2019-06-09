#include "MySaverBase.h"
#include "MySaveLoadState.h"
#include "MySaveLoadSystemUtils.h"
#include "SaveLoad/IMySaveable.h"
#include "SaveLoad/IMySaveableHandle.h"
#include "SaveLoad/MySaveableUtils.h"
#include "PerObjectSaveLoadData.h"
#include "../Sys/IMySaveLoadSystemInternal.h"

#include "Util/Core/LogUtilLib.h"

#include "Engine/GameInstance.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerController.h"

#include "EngineUtils.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

void UMySaverBase::SetupSaverBase(IMySaveLoadSystemInternal* const InSys, FArchive* const InArchive, UWorld* const InWorld, UMySaveLoadState* const InState)
{
	UE_LOG(MyLog, Log, TEXT("SetupSaverBase..."));

	SetupSaverLoaderBase(ESaverOrLoader::Saver, InSys, InArchive, InWorld, InState);

	UE_LOG(MyLog, Log, TEXT("SetupSaverBase DONE"));
}

void UMySaverBase::Save()
{
	UE_LOG(MyLog, Log, TEXT("UMySaverBase::Save..."));

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

	UE_LOG(MyLog, Log, TEXT("UMySaverBase::Save DONE"));
}

void UMySaverBase::Extract_WorldInfo()
{
	UE_LOG(MyLog, Log, TEXT("Extracing info about the world..."));

	{
		UE_LOG(MyLog, Log, TEXT("Extracing map name..."));

		FString MapName, MapName_NoStreamingPrefix;

		{
			MapName = GetWorld()->GetMapName();
			UE_LOG(MyLog, Log, TEXT("GetWorld()->GetMapName() returned \"%s\""), *MapName);
		}

	
		{
			MapName_NoStreamingPrefix = MapName;
			MapName_NoStreamingPrefix.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

			GetState()->MapName = MapName_NoStreamingPrefix;

			UE_LOG(MyLog, Log, TEXT("MapName(no streaming prefix): \"%s\""), *MapName_NoStreamingPrefix);
		}

		UE_LOG(MyLog, Log, TEXT("Extracing map name DONE"));
	}

	UE_LOG(MyLog, Log, TEXT("Extracing info about the world DONE"));
}

void UMySaverBase::Extract_DestructedObjects()
{
	UE_LOG(MyLog, Log, TEXT("Extracing info about destructed objects..."));
	GetState()->StaticDestructedObjects = GetSys()->GetStaticDestructedObjects();
	UE_LOG(MyLog, Log, TEXT("%d destructed objects found"), GetState()->StaticDestructedObjects.Num());
	for(const FName& DestructObjectName : GetState()->StaticDestructedObjects)
	{
		UE_LOG(MyLog, Log, TEXT("DestructedObject: named \"%s\""), *DestructObjectName.ToString());
	}
	UE_LOG(MyLog, Log, TEXT("Extracing info about destructed objects DONE"));
}

void UMySaverBase::BinarizeToArchive()
{
	UE_LOG(MyLog, Log, TEXT("Binarizing to archive..."));
	UMySaveLoadSystemUtils::SaveLoadStateToSaveStruct(this, &GetBinaryWorld(), GetState());
	SerializeToFromArchive();
	UE_LOG(MyLog, Log, TEXT("Binarizing to archive DONE"));
}

void UMySaverBase::Find_WorldObjects()
{
	UE_LOG(MyLog, Log, TEXT("UMySaverBase::Find_WorldObjects..."));

	int32 NumGlobalObjectsInWorld = 0;
	for(TArray<TScriptInterface<IMySaveableHandle>>::TConstIterator Itr = GetSys()->CreateSaveableHandleIterator(); Itr; ++Itr)
	{
		if(ShouldObjectBeSaved(*Itr, /*bLogged=*/true, /*bLogOnFalseOnly=*/true))
		{
			if( false == IsGlobalObject(*Itr) )	
			{
				GetState()->WorldSaveableHandles.AddUnique(*Itr);
			}
			else
			{
				UE_LOG(MyLog, Log, TEXT("Global object \"%s\" found"), *(*Itr)->SaveLoad_ToString());
				NumGlobalObjectsInWorld++;
			}
		}		
	}
	UE_LOG(MyLog, Log, TEXT("%d saveable actors found, %d global objects in world"), GetState()->WorldSaveableHandles.Num(), NumGlobalObjectsInWorld);

	UE_LOG(MyLog, Log, TEXT("UMySaverBase::Find_WorldObjects DONE"));
}

void UMySaverBase::Find_GlobalObjects()
{
	UE_LOG(MyLog, Log, TEXT("UMySaverBase::Find_GlobalObjects..."));

	Find_GlobalObject_PlayerController();
	Find_GlobalObject_GameMode();
	Find_GlobalObject_GameInstance();

	UE_LOG(MyLog, Log, TEXT("Total %d global objects registered"), GetState()->GlobalSaveableHandles.Num());
	UE_LOG(MyLog, Log, TEXT("UMySaverBase::Find_GlobalObjects DONE"));
}


void UMySaverBase::Find_GlobalObject_PlayerController()
{
	UE_LOG(MyLog, Log, TEXT("Find_GlobalObject_PlayerController..."));
	
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if(PC == nullptr)
	{
		UE_LOG(MyLog, Log, TEXT("UGameplayStatics::GetPlayerController() returned nullptr!"));
	}
	else
	{
		RegisterGlobalObject_IfShouldBeSaved(PC);
	}

	UE_LOG(MyLog, Log, TEXT("Find_GlobalObject_PlayerController DONE"));
}

void UMySaverBase::Find_GlobalObject_GameMode()
{
	UE_LOG(MyLog, Log, TEXT("Find_GlobalObject_GameMode..."));
	
	AGameModeBase* G = GetWorld()->GetAuthGameMode();
	if(G == nullptr)
	{
		UE_LOG(MyLog, Log, TEXT("GetAuthGameMode() returned nullptr!"));
	}
	else
	{
		RegisterGlobalObject_IfShouldBeSaved(G);
	}

	UE_LOG(MyLog, Log, TEXT("Find_GlobalObject_GameMode DONE"));
}

void UMySaverBase::Find_GlobalObject_GameInstance()
{
	UE_LOG(MyLog, Log, TEXT("Find_GlobalObject_GameInstance..."));

	UGameInstance* const G = GetWorld()->GetGameInstance();
	RegisterGlobalObject_IfShouldBeSaved(G);

	UE_LOG(MyLog, Log, TEXT("Find_GlobalObject_GameInstance DONE"));
}

void UMySaverBase::Extract_ClassTable()
{
	UE_LOG(MyLog, Log, TEXT("UMySaverBase::ExtractClassTable..."));
	
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

	UE_LOG(MyLog, Log, TEXT("UMySaverBase::ExtractClassTable DONE"));
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
	UE_LOG(MyLog, Log, TEXT("UMySaverBase::RegisterGlobalObject..."));
	check(InSaveableHandle);
	UE_LOG(MyLog, Log, TEXT("Object \"%s\""), *InSaveableHandle->SaveLoad_ToString());
	bool const bAdded = (1 == GetState()->GlobalSaveableHandles.AddUnique(InSaveableHandle));
	checkf(bAdded, TEXT("Saveable handle must be successfully added to the container"));
	UE_LOG(MyLog, Log, TEXT("UMySaverBase::RegisterGlobalObject DONE"));
}

void UMySaverBase::RegisterGlobalObject_IfShouldBeSaved(UObject* const InObject)
{
	UE_LOG(MyLog, Log, TEXT("UMySaverBase::RegisterGlobalObject_IfShouldBeSaved..."));

	if(InObject == nullptr)
	{
		UE_LOG(MyLog, Log, TEXT("Skipping object registration - passed object is nullptr"));
		return;
	}

	if(IMySaveable* Saveable = Cast<IMySaveable>(InObject))
	{
		TScriptInterface<IMySaveableHandle> const SaveableHandle = Saveable->SaveLoad_GetHandle();
		checkf(SaveableHandle, TEXT("SaveableHandle should never be nullptr"));
		UE_LOG(MyLog, Log, TEXT("Object \"%s\""), *SaveableHandle->SaveLoad_ToString());

		if(ShouldObjectBeSaved(SaveableHandle, /*bLogged=*/true))
		{
			RegisterGlobalObject(SaveableHandle);
		}
	}
	else
	{
		checkf(false, TEXT("Skipping registration - saveable interface is not supported"));
	}
}