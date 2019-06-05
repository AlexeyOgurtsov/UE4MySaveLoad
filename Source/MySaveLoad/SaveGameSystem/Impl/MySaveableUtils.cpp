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
	if(false == InObj->GetUniqueFName().IsNone())
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
	if( false == InObj->IsSaveLoad() )
	{
		return true;
	}

	if(InObj->IsDynamic() && InObj->IsGlobal())
	{
		if(bInLogged)
		{
			UE_LOG(MyLog, Error, TEXT("Global objects cannot be marked as dynamic!"));
		}
		return false;
	}

	return true;
}