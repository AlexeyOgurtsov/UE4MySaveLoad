#include "MySaveLoadSystemUtils.h"
#include "MySaveLoadState.h"
#include "PerObjectSaveLoadData.h"

#include "SaveLoad/IMySaveable.h"
#include "SaveLoad/IMySaveableHandle.h"
#include "SaveLoad/MySaveableUtils.h"

#include "SaveLoad/Util/MySaveArchive.h"
#include "Util/Core/LogUtilLib.h"

#include "Serialization/MemoryWriter.h"

namespace
{
	FString const FORMAT_SIGNATURE { "SAV" };
} // anonymous

void UMySaveLoadSystemUtils::CheckSavedWorldValid(const FMySavedWorld* const InWorld)
{
	check(IsSavedWorldValid(InWorld, /*bLogged=*/true));
}

bool UMySaveLoadSystemUtils::IsSavedWorldValid(const FMySavedWorld* const InWorld, bool const bInLogged)
{
	if(IsSavedWorldMetaValid(&InWorld->FormatMeta, bInLogged))
	{
		return false;
	}

	return true;
}

bool UMySaveLoadSystemUtils::IsSavedWorldMetaValid(const FMySaveFormatMeta* const InMeta, bool const bInLogged)
{
	if(InMeta->Signature != FORMAT_SIGNATURE)
	{		
		if(bInLogged)
		{
			UE_LOG(MyLog, Error, TEXT("Format signature is wrong (\"%s\" found, but \"%s\" required)"), *InMeta->Signature, *FORMAT_SIGNATURE);
		}
		return false;
	}
	return true;
}

bool UMySaveLoadSystemUtils::IsSavedObjectValid(UMySaverLoaderBase* const InSender, const FMySavedObject* const InSavedObj, const FMySavedWorld* const InWorld, bool const bInLogged)
{
	check(InSavedObj);

	const FString& ObjectUniqueName = InSavedObj->UniqueName;
	int32 const ClassIndex = InSavedObj->ClassIndex;

	// First of all check object's class index
	if(false == IsSavedClassIndexValid(InSender, InWorld, ClassIndex, bInLogged, FString::Printf(TEXT("Obj with UniqueName \"%s\" :"), *ObjectUniqueName)))
	{
		return false;
	}

	const FString& ClassName = InWorld->Classes[ClassIndex].Name;
	
	TArray<FStringFormatArg> FormatArgs;
	FormatArgs.Add(ObjectUniqueName);	
	FormatArgs.Add(ClassName);
	FString PrefixString = FString::Format(TEXT("Check of object with UniqueName \"{0}\" of class \"{1}\""), FormatArgs);

	return true;
}

bool UMySaveLoadSystemUtils::IsSavedClassIndexValid(UMySaverLoaderBase* const InSender, const FMySavedWorld* const InWorld, int32 const InClassIndex, bool const bInLogged, const FString& LogPrefix)
{
	// First of all check object's class index
	if(InClassIndex < 0)
	{
		if(bInLogged)
		{
			UE_LOG(MyLog, Error, TEXT("%sClass index is negative"), *LogPrefix);
		}
		return false;
	}

	if(InClassIndex >= InWorld->Classes.Num())
	{
		if(bInLogged)
		{
			UE_LOG(MyLog, Error, TEXT("%sClass index is greater than all classes"), *LogPrefix);
		}
		return false;
	}

	return true;
}

void UMySaveLoadSystemUtils::PrepareFormatMetaSaveStruct(UMySaverLoaderBase* InSender, FMySaveFormatMeta* const InSavedFormatMeta)
{
	UE_LOG(MyLog, Log, TEXT("Preparing save format meta..."));
	InSavedFormatMeta->Signature = FORMAT_SIGNATURE;
	UE_LOG(MyLog, Log, TEXT("Signature: \"%s\""), *InSavedFormatMeta->Signature);
	UE_LOG(MyLog, Log, TEXT("Preparing save format meta DONE"));
}

void UMySaveLoadSystemUtils::PrepareWorldInfoSaveStruct(UMySaverLoaderBase* InSender, FMySavedWorldInfo* const InSavedWorldInfo, const UMySaveLoadState* const InSaveLoadState)
{
	UE_LOG(MyLog, Log, TEXT("Preparing world info save struct..."));
	InSavedWorldInfo->MapName = InSaveLoadState->MapName;
	UE_LOG(MyLog, Log, TEXT("MapName: \"%s\""), *InSavedWorldInfo->MapName);
	UE_LOG(MyLog, Log, TEXT("Preparing world info save struct DONE"));
}

void UMySaveLoadSystemUtils::PrepareObjectDestructStruct(UMySaverLoaderBase* const InSender, FMySavedDestruct* const InSavedDestruct, const FName& InUniqueName)
{
	InSavedDestruct->UniqueName = InUniqueName.ToString();
}

void UMySaveLoadSystemUtils::PrepareDestructedObjects_FromLoadState(UMySaverLoaderBase* const InSender, TArray<FMySavedDestruct>* const InSavedDestructedObjects, const TArray<FName>& InDestructedObjects)
{
	InSavedDestructedObjects->Reserve(InDestructedObjects.Num());
	for(FName DestructedName : InDestructedObjects)
	{
		FMySavedDestruct SavedDestruct;
		PrepareObjectDestructStruct(InSender, &SavedDestruct, DestructedName);
		InSavedDestructedObjects->Add(SavedDestruct);
	}
	UE_LOG(MyLog, Log, TEXT("%d Destructed objects loaded from LoadState"), InSavedDestructedObjects->Num());
}

void UMySaveLoadSystemUtils::PrepareObjectSaveStruct(UMySaverLoaderBase* const InSender, FMySavedObject* const InSavedObj, TScriptInterface<IMySaveableHandle> const InSaveableHandle)
{
	check(InSavedObj);
	check(InSaveableHandle);
	auto ObjData = Cast<UPerObjectSaveLoadData>(InSaveableHandle->SaveLoad_GetData(InSender));
	checkf(ObjData, TEXT("Obj data for object %s must be of class \"%s\""), *InSaveableHandle->SaveLoad_ToString(), *UPerObjectSaveLoadData::StaticClass()->GetName());
	InSavedObj->ClassIndex = ObjData->ClassIndex;
	InSavedObj->UniqueName = InSaveableHandle->SaveLoad_GetUniqueName();
}

void UMySaveLoadSystemUtils::PrepareClassSaveStruct(UMySaverLoaderBase* const InSender, FMySavedClass* const InSavedClass, UClass* const InClass)
{
	check(InSavedClass);
	check(InClass);
	InSavedClass->Name = InClass->GetName();
}

void UMySaveLoadSystemUtils::SaveLoadStateToSaveStruct(UMySaverLoaderBase* const InSender, FMySavedWorld* const InSavedWorld, const UMySaveLoadState* const InSaveLoadState)
{
	check(InSaveLoadState);
	check(InSavedWorld);

	PrepareFormatMetaSaveStruct(InSender, &InSavedWorld->FormatMeta);

	PrepareWorldInfoSaveStruct(InSender, &InSavedWorld->WorldInfo, InSaveLoadState);

	PrepareSaveClasses_FromLoadState(InSender, InSavedWorld, InSaveLoadState);
	PrepareSaveObjects_FromLoadState(InSender, &InSavedWorld->GlobalObjects, InSaveLoadState->GlobalSaveableHandles);
	PrepareSaveObjects_FromLoadState(InSender, &InSavedWorld->WorldObjects, InSaveLoadState->WorldSaveableHandles);

	PrepareDestructedObjects_FromLoadState(InSender, &InSavedWorld->StaticDestructedObjects, InSaveLoadState->StaticDestructedObjects);
}

void UMySaveLoadSystemUtils::PrepareSaveClasses_FromLoadState(UMySaverLoaderBase* const InSender, FMySavedWorld* const InSavedWorld, const UMySaveLoadState* const InSaveLoadState)
{
	check(InSaveLoadState);
	check(InSavedWorld);
	const TArray<UClass*>& Classes = InSaveLoadState->Classes;

	InSavedWorld->Classes.Reserve(Classes.Num());
	for(int32 Index = 0; Index < Classes.Num(); Index++)
	{
		FMySavedClass SavedClass;
		PrepareClassSaveStruct(InSender, &SavedClass, Classes[Index]);
		InSavedWorld->Classes.Add(SavedClass);
	}
}

void UMySaveLoadSystemUtils::PrepareSaveObjects_FromLoadState(UMySaverLoaderBase* const InSender, TArray<FMySavedObject>* const InSavedObjects, const TArray<TScriptInterface<IMySaveableHandle>>& InSaveableHandles)
{
	check(InSavedObjects);

	InSavedObjects->Reserve(InSaveableHandles.Num());
	for(int32 Index = 0; Index < InSaveableHandles.Num(); Index++)
	{
		FMySavedObject SavedObject;
		TScriptInterface<IMySaveableHandle> SaveableHandle = InSaveableHandles[Index];

		PrepareObjectSaveStruct(InSender, &SavedObject, SaveableHandle);
		PrepareObjectSaveData(InSender, &SavedObject, SaveableHandle);

		InSavedObjects->Add(SavedObject);
	}
}

void UMySaveLoadSystemUtils::PrepareObjectSaveData(UMySaverLoaderBase* const InSender, FMySavedObject* const InSavedObj, TScriptInterface<IMySaveableHandle> const InSaveableHandle)
{
	check(InSaveableHandle);
	TArray<uint8>& SavedData = InSavedObj->Data;

	FMemoryWriter MemoryWriter { SavedData, /*bPersistent=*/true };
	FMySaveArchive Ar { MemoryWriter };
	
	InSaveableHandle->SaveLoad_GetSaveable()->SaveLoad_Serialize(Ar);
}