#include "SaveGameSystem/MySaveableUtils.h"
#include "SaveGameSystem/IMySaveable.h"
#include "SaveGameSystem/Util/MySaveArchive.h"

#include "Util/Core/LogUtilLib.h"

void UMySaveableUtils::SerializeObjectData(FArchive& Ar, UObject* InObj)
{
	InObj->Serialize(Ar);
}

bool UMySaveableUtils::IsSaveableValid(TScriptInterface<IMySaveable> InSaveable, bool bInLogged)
{
	if(false == InSaveable->SaveLoad_GetUniqueFName().IsNone())
	{
		if(bInLogged)
		{
			UE_LOG(MyLog, Error, TEXT("UniqueName must always be set for saveable object!"));
		}
		return false;
	}

	if(false == AreSaveableFlagsValid(InSaveable, bInLogged))	
	{
		return false;
	}

	return true;
}

bool UMySaveableUtils::AreSaveableFlagsValid(TScriptInterface<IMySaveable> InSaveable, bool bInLogged)
{
	// When SaveLoad is disabled, any combination of flags is valid
	if( false == InSaveable->SaveLoad_IsEnabled() )
	{
		return true;
	}

	if(InSaveable->SaveLoad_IsDynamic() && InSaveable->SaveLoad_IsGlobal())
	{
		if(bInLogged)
		{
			UE_LOG(MyLog, Error, TEXT("Global objects cannot be marked as dynamic!"));
		}
		return false;
	}

	return true;
}