#include "MySaveLoadSystemUtils.h"
#include "MySaveLoadState.h"
#include "PerObjectSaveLoadData.h"

#include "SaveGameSystem/IMySaveable.h"
#include "SaveGameSystem/MySaveableUtils.h"

#include "SaveGameSystem/Util/MySaveArchive.h"
#include "Util/Core/LogUtilLib.h"

#include "Serialization/MemoryWriter.h"

namespace
{
	FString const FORMAT_SIGNATURE { "SAV" };
} // anonymous

void UMySaveLoadSystemUtils::CheckSavedWorldValid(const FMySavedWorld* InWorld)
{
	check(IsSavedWorldValid(InWorld, /*bLogged=*/true));
}

bool UMySaveLoadSystemUtils::IsSavedWorldValid(const FMySavedWorld* InWorld, bool bInLogged)
{
	if(IsSavedWorldMetaValid(&InWorld->FormatMeta, bInLogged))
	{
		return false;
	}

	return true;
}

bool UMySaveLoadSystemUtils::IsSavedWorldMetaValid(const FMySaveFormatMeta* InMeta, bool bInLogged)
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

bool UMySaveLoadSystemUtils::IsSavedObjectValid(UMySaverLoaderBase* InSender, const FMySavedObject* InSavedObj, const FMySavedWorld* InWorld, bool bInLogged)
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

bool UMySaveLoadSystemUtils::IsSavedClassIndexValid(UMySaverLoaderBase* InSender, const FMySavedWorld* InWorld, int32 InClassIndex, bool bInLogged, const FString& LogPrefix)
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

void UMySaveLoadSystemUtils::PrepareFormatMetaSaveStruct(UMySaverLoaderBase* InSender, FMySaveFormatMeta* InSavedFormatMeta)
{
	UE_LOG(MyLog, Log, TEXT("Preparing save format meta..."));
	InSavedFormatMeta->Signature = FORMAT_SIGNATURE;
	UE_LOG(MyLog, Log, TEXT("Signature: \"%s\""), *InSavedFormatMeta->Signature);
	UE_LOG(MyLog, Log, TEXT("Preparing save format meta DONE"));
}

void UMySaveLoadSystemUtils::PrepareWorldInfoSaveStruct(UMySaverLoaderBase* InSender, FMySavedWorldInfo* InSavedWorldInfo, const UMySaveLoadState* InSaveLoadState)
{
	UE_LOG(MyLog, Log, TEXT("Preparing world info save struct..."));
	InSavedWorldInfo->MapName = InSaveLoadState->MapName;
	UE_LOG(MyLog, Log, TEXT("MapName: \"%s\""), *InSavedWorldInfo->MapName);
	UE_LOG(MyLog, Log, TEXT("Preparing world info save struct DONE"));
}

void UMySaveLoadSystemUtils::PrepareObjectDestructStruct(UMySaverLoaderBase* InSender, FMySavedDestruct* InSavedDestruct, const FName& InUniqueName)
{
	InSavedDestruct->UniqueName = InUniqueName.ToString();
}

void UMySaveLoadSystemUtils::PrepareDestructedObjects_FromLoadState(UMySaverLoaderBase* InSender, TArray<FMySavedDestruct>* InSavedDestructedObjects, const TArray<FName>& InDestructedObjects)
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

void UMySaveLoadSystemUtils::PrepareObjectSaveStruct(UMySaverLoaderBase* InSender, FMySavedObject* InSavedObj, TScriptInterface<IMySaveable> InObj)
{
	check(InSavedObj);
	check(InObj);
	FString const ObjectUniqueName = UMySaveableUtils::GetUniqueName(InObj.GetObject()).ToString();
	auto ObjData = Cast<UPerObjectSaveLoadData>(InObj->SaveLoad_GetData(InSender));
	checkf(ObjData, TEXT("Obj data for object \"%s\" with unique name \"%s\" of class \"%s\" must be of class \"PerObjectSaveLoadData\""), *InObj.GetObject()->GetName(), *ObjectUniqueName, *InObj.GetObject()->GetClass()->GetName());
	InSavedObj->ClassIndex = ObjData->ClassIndex;
	InSavedObj->UniqueName = ObjectUniqueName;
}

void UMySaveLoadSystemUtils::PrepareClassSaveStruct(UMySaverLoaderBase* InSender, FMySavedClass* InSavedClass, UClass* InClass)
{
	check(InSavedClass);
	check(InClass);
	InSavedClass->Name = InClass->GetName();
}

void UMySaveLoadSystemUtils::SaveLoadStateToSaveStruct(UMySaverLoaderBase* InSender, FMySavedWorld* InSavedWorld, const UMySaveLoadState* InSaveLoadState)
{
	check(InSaveLoadState);
	check(InSavedWorld);

	PrepareFormatMetaSaveStruct(InSender, &InSavedWorld->FormatMeta);

	PrepareWorldInfoSaveStruct(InSender, &InSavedWorld->WorldInfo, InSaveLoadState);

	PrepareSaveClasses_FromLoadState(InSender, InSavedWorld, InSaveLoadState);
	PrepareSaveObjects_FromLoadState(InSender, &InSavedWorld->GlobalObjects, InSaveLoadState->GlobalObjects);
	PrepareSaveObjects_FromLoadState(InSender, &InSavedWorld->WorldObjects, InSaveLoadState->WorldObjects);

	PrepareDestructedObjects_FromLoadState(InSender, &InSavedWorld->DestructedObjects, InSaveLoadState->DestructedObjects);
}

void UMySaveLoadSystemUtils::PrepareSaveClasses_FromLoadState(UMySaverLoaderBase* InSender, FMySavedWorld* InSavedWorld, const UMySaveLoadState* InSaveLoadState)
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

void UMySaveLoadSystemUtils::PrepareSaveObjects_FromLoadState(UMySaverLoaderBase* InSender, TArray<FMySavedObject>* InSavedObjects, const TArray<TScriptInterface<IMySaveable>>& InObjects)
{
	check(InSavedObjects);

	InSavedObjects->Reserve(InObjects.Num());
	for(int32 Index = 0; Index < InObjects.Num(); Index++)
	{
		FMySavedObject SavedObject;
		TScriptInterface<IMySaveable> Obj = InObjects[Index];

		PrepareObjectSaveStruct(InSender, &SavedObject, Obj);
		PrepareObjectSaveData(InSender, &SavedObject, Obj);

		InSavedObjects->Add(SavedObject);
	}
}

void UMySaveLoadSystemUtils::PrepareObjectSaveData(UMySaverLoaderBase* InSender, FMySavedObject* InSavedObj, TScriptInterface<IMySaveable> InObj)
{
	check(InObj);
	TArray<uint8>& SavedData = InSavedObj->Data;

	FMemoryWriter MemoryWriter { SavedData, /*bPersistent=*/true };
	FMySaveArchive Ar { MemoryWriter };
	
	InObj->SaveLoad_Serialize(Ar);
}