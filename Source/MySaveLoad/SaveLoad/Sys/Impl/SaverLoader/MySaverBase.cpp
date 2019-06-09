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
				GetState()->WorldObjects.AddUnique((*Itr)->SaveLoad_GetSaveable());
			}
			else
			{
				UE_LOG(MyLog, Log, TEXT("Global object \"%s\" found"), *(*Itr)->SaveLoad_ToString());
				NumGlobalObjectsInWorld++;
			}
		}		
	}
	UE_LOG(MyLog, Log, TEXT("%d saveable actors found, %d global objects in world"), GetState()->WorldObjects.Num(), NumGlobalObjectsInWorld);

	UE_LOG(MyLog, Log, TEXT("UMySaverBase::Find_WorldObjects DONE"));
}

void UMySaverBase::Find_GlobalObjects()
{
	UE_LOG(MyLog, Log, TEXT("UMySaverBase::Find_GlobalObjects..."));

	Find_GlobalObject_PlayerController();
	Find_GlobalObject_GameMode();
	Find_GlobalObject_GameInstance();

	UE_LOG(MyLog, Log, TEXT("Total %d global objects registered"), GetState()->GlobalObjects.Num());
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

	UGameInstance* G = GetWorld()->GetGameInstance();
	if(G == nullptr)
	{
		UE_LOG(MyLog, Log, TEXT("GetGameInstance() returned nullptr!"));
	}
	else
	{
		RegisterGlobalObject_IfShouldBeSaved(G);
	}
	UE_LOG(MyLog, Log, TEXT("Find_GlobalObject_GameInstance DONE"));
}

void UMySaverBase::Extract_ClassTable()
{
	UE_LOG(MyLog, Log, TEXT("UMySaverBase::ExtractClassTable..."));
	
	for(TScriptInterface<IMySaveable> O : GetState()->GlobalObjects)
	{
		GetState()->Classes.AddUnique(O.GetObject()->GetClass());
	}

	for(TScriptInterface<IMySaveable> O : GetState()->WorldObjects)
	{
		GetState()->Classes.AddUnique(O.GetObject()->GetClass());
	}

	UE_LOG(MyLog, Log, TEXT("Total %d classes extracted"), GetState()->Classes.Num());

	BindClassIndicesToObjects(GetState()->GlobalObjects);
	BindClassIndicesToObjects(GetState()->WorldObjects);

	UE_LOG(MyLog, Log, TEXT("UMySaverBase::ExtractClassTable DONE"));
}

void UMySaverBase::BindClassIndicesToObjects(const TArray<TScriptInterface<IMySaveable>>& InObjects)
{
	for(TScriptInterface<IMySaveable> Obj : InObjects)
	{
		check(Obj);
		int32 ClassIndex;
		bool bClassFound = GetState()->Classes.Find(Obj.GetObject()->GetClass(), /*Out*/ClassIndex);
		checkf(bClassFound, TEXT("Class for object \"%s\" of class \"%s\" must be registered in the class table"), *Obj.GetObject()->GetName(), *Obj.GetObject()->GetClass()->GetName());
		UPerObjectSaveLoadDataBase* const ObjDataBase = Obj->SaveLoad_GetHandle()->SaveLoad_GetData(this);
		checkf(bClassFound, TEXT("Class for object \"%s\" of class \"%s\": Saveable data object must be assigned!"), *Obj.GetObject()->GetName(), *Obj.GetObject()->GetClass()->GetName());
		auto ObjData = CastChecked<UPerObjectSaveLoadData>(ObjDataBase);
		ObjData->ClassIndex = ClassIndex;
		
	}
}

void UMySaverBase::RegisterGlobalObject(UObject* const InObject)
{
	UE_LOG(MyLog, Log, TEXT("UMySaverBase::RegisterGlobalObject..."));
	check(InObject);
	UE_LOG(MyLog, Log, TEXT("Object named \"%s\" of class \"%s\""), *InObject->GetName(), *InObject->GetClass()->GetName());
	GetState()->GlobalObjects.AddUnique(TScriptInterface<IMySaveable>(InObject));
	UE_LOG(MyLog, Log, TEXT("UMySaverBase::RegisterGlobalObject DONE"));
}

void UMySaverBase::RegisterGlobalObject_IfShouldBeSaved(UObject* const InObject)
{
	UE_LOG(MyLog, Log, TEXT("UMySaverBase::RegisterGlobalObject_IfShouldBeSaved..."));
	check(InObject);
	UE_LOG(MyLog, Log, TEXT("Object named \"%s\" of class \"%s\""), *InObject->GetName(), *InObject->GetClass()->GetName());

	if(ShouldObjectBeSaved(InObject, /*bLogged=*/true))
	{
		RegisterGlobalObject(InObject);
	}

	UE_LOG(MyLog, Log, TEXT("UMySaverBase::RegisterGlobalObject_IfShouldBeSaved DONE"));
}