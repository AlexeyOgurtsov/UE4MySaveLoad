#include "SaveLoad/MySaveableUtils.h"
#include "SaveLoad/IMySaveable.h"
#include "SaveLoad/Util/MySaveArchive.h"

#include "SaveLoad/Util/SaveLoadLogUtilLib.h"

void UMySaveableUtils::Default_Serialize(TScriptInterface<IMySaveable> const InSaveable, FArchive& Ar)
{
	SL_LOG(TEXT("%s Default_Serialize is called"), *InSaveable->SaveLoad_ToString());
	SerializeObjectData(Ar, InSaveable.GetObject());
}

void UMySaveableUtils::Default_AllObjectsLoaded(TScriptInterface<IMySaveable> const InSaveable, FArchive& Ar)
{
	SL_EMPTY();
}

void UMySaveableUtils::Default_BeforeDestroy(TScriptInterface<IMySaveable> const InSaveable)
{
	SL_EMPTY();
}

void UMySaveableUtils::SerializeObjectData(FArchive& Ar, UObject* const InObj)
{
	InObj->Serialize(Ar);
}

bool UMySaveableUtils::IsSaveableValid(TScriptInterface<IMySaveable> const InSaveable, bool const bInLogged)
{
	if(false == InSaveable->SaveLoad_GetUniqueFName().IsNone())
	{
		if(bInLogged)
		{
			SL_LOG_ERROR(TEXT("%s: UniqueName must always be set for saveable object"), *InSaveable->SaveLoad_ToString());
		}
		return false;
	}

	if(false == AreSaveableFlagsValid(InSaveable, bInLogged))	
	{
		return false;
	}

	return true;
}

bool UMySaveableUtils::AreSaveableFlagsValid(TScriptInterface<IMySaveable> const InSaveable, const bool bInLogged)
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
			SL_LOG_ERROR(TEXT("%s: Global objects cannot be marked as dynamic"), *InSaveable->SaveLoad_ToString());
		}
		return false;
	}

	return true;
}