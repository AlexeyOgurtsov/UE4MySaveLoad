#include "SaveLoad/MySaveableUtils.h"
#include "SaveLoad/IMySaveable.h"
#include "SaveLoad/Util/MySaveArchive.h"

#include "SaveLoad/Util/SaveLoadLogUtilLib.h"
#include "Util/Property/PropertyLogLib.h"
#include "Util/Property/PropertyLib.h"

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

bool UMySaveableUtils::IsSaveableValid(TScriptInterface<IMySaveable> const InSaveable, ELogFlags InLogFlags)
{
	SL_LOGFUNC_IF_FLAGS(InLogFlags);
	checkNoRecursion();

	if(false == InSaveable->SaveLoad_GetUniqueFName().IsNone())
	{
		SL_LOG_ERROR_IF_FLAGS(InLogFlags, TEXT("%s: UniqueName must always be set for saveable object"), *InSaveable->SaveLoad_ToString());
		return false;
	}

	if(false == AreSaveableFlagsValid(InSaveable, InLogFlags))	
	{
		return false;
	}

	if(false == AreAllFieldsValidRecursive(InSaveable, InLogFlags))
	{
		return false;
	}

	return true;
}

bool UMySaveableUtils::AreSaveableFlagsValid(TScriptInterface<IMySaveable> const InSaveable, ELogFlags InLogFlags)
{
	SL_LOGFUNC_IF_FLAGS(InLogFlags);
	checkNoRecursion();

	// When SaveLoad is disabled, any combination of flags is valid
	if( false == InSaveable->SaveLoad_IsEnabled() )
	{
		return true;
	}

	if(InSaveable->SaveLoad_IsDynamic() && InSaveable->SaveLoad_IsGlobal())
	{
		SL_LOG_ERROR_IF_FLAGS(InLogFlags, TEXT("%s: Global objects cannot be marked as dynamic"), *InSaveable->SaveLoad_ToString());
		return false;
	}

	return true;
}

bool UMySaveableUtils::AreAllFieldsValidRecursive(TScriptInterface<IMySaveable> InSaveable, ELogFlags InLogFlags)
{
	if( ! InSaveable )
	{
		return true;
	}

	for(const TPair<const UProperty*, const void*>& PropValuePair : TPropertyValueRange<const UProperty>(InSaveable.GetObject()->GetClass(), InSaveable.GetObject(), EPropertyValueIteratorFlags::FullRecursion))
	{
		const UProperty* Prop = PropValuePair.Key;
		const void* PropValue = PropValuePair.Value;
		FString PropertyPrefix = FString::Printf(TEXT("property {%s}: "), *UPropertyLogLib::GetPropertyValueString(Prop, PropValue));

		if( Prop->HasAllPropertyFlags(CPF_SaveGame) )
		{
			M_LOG_IF_FLAGS(InLogFlags, TEXT("%sSaveGame flag set"), *PropertyPrefix);

			const UObject* PropValueAsObject = UPropertyLib::GetPropertyValueAsObject(Prop, PropValue);
			if(PropValueAsObject)
			{
				M_LOG_IF_FLAGS(InLogFlags, TEXT("%sProperty value is valid UObject"), *PropertyPrefix);
				if(PropValueAsObject->GetClass()->ImplementsInterface(UMySaveable::StaticClass()))
				{
					SL_LOG_ERROR_IF_FLAGS(InLogFlags, TEXT("%sSubobject of %s-implementing object not allowed to implement the saveable interface itself when its property is marked with the SaveLoad flag."), *PropertyPrefix, *UMySaveable::StaticClass()->GetName());
					return false;
				}
			}
		}
	}

	return true;
}
