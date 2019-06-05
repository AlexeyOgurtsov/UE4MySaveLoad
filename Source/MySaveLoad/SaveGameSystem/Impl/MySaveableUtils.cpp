#include "SaveGameSystem/MySaveableUtils.h"
#include "SaveGameSystem/IMySaveable.h"
#include "SaveGameSystem/Util/MySaveArchive.h"

#include "Util/Core/LogUtilLib.h"

void UMySaveableUtils::SerializeObjectData(FArchive& Ar, UObject* InObj)
{
	InObj->Serialize(Ar);
}

bool UMySaveableUtils::IsSaveableValid(TScriptInterface<IMySaveable> InObj, bool bInLogged)
{
	if(false == GetUniqueName(InObj).IsNone() && false == GetUniqueName(InObj).ToString().IsEmpty())
	{
		if(bInLogged)
		{
			UE_LOG(MyLog, Error, TEXT("UniqueName must always be set for saveable object!"));
		}
		return false;
	}

	if(false == AreSaveableFlagsValid(InObj, bInLogged))	
	{
		return false;
	}

	return true;
}

bool UMySaveableUtils::AreSaveableFlagsValid(TScriptInterface<IMySaveable> InObj, bool bInLogged)
{
	// When SaveLoad is disabled, any combination of flags is valid
	if( false == IsSaveLoad(InObj) )
	{
		return true;
	}

	if(IsDynamic(InObj) && IsGlobal(InObj))
	{
		if(bInLogged)
		{
			UE_LOG(MyLog, Error, TEXT("Global objects cannot be marked as dynamic!"));
		}
		return false;
	}

	return true;
}

const FName& UMySaveableUtils::GetUniqueName(TScriptInterface<IMySaveable> InObj)
{
	const FMySaveableStaticProps& Props = InObj->SaveLoad_GetStaticProps();
	return Props.UniqueName;
}

bool UMySaveableUtils::IsGlobal(TScriptInterface<IMySaveable> InObj)
{
	const FMySaveablePerClassProps& Props = InObj->SaveLoad_GetClassProps();

	return (Props.Flags & EMySaveablePerClassFlags::GlobalObject) != EMySaveablePerClassFlags::None;
}

bool UMySaveableUtils::IsSaveLoad(TScriptInterface<IMySaveable> InObj)
{
	const FMySaveableStaticProps& Props = InObj->SaveLoad_GetStaticProps();

	return (Props.Flags & EMySaveableStaticFlags::SaveLoad) != EMySaveableStaticFlags::None;
}

bool UMySaveableUtils::IsDynamic(TScriptInterface<IMySaveable> InObj)
{
	const FMySaveableStaticProps& Props = InObj->SaveLoad_GetStaticProps();

	return (Props.Flags & EMySaveableStaticFlags::Dynamic) != EMySaveableStaticFlags::None;
}

bool UMySaveableUtils::IsStatic(TScriptInterface<IMySaveable> InObj)
{
	return false == IsDynamic(InObj);
}