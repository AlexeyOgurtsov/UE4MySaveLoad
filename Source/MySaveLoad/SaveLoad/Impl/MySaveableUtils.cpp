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

bool UMySaveableUtils::IsSaveableValid(TScriptInterface<IMySaveable> const InSaveable, ELogFlags InLogFlags)
{
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

namespace
{
	//class FFieldVisitState
	//{
	//public:
		/**
		* All fields (already visited and to be visited)
		*/
	//	const TArray<const UField*>& GetFields() const { return Fields; }

		/**
		* All objects that were visited, and, so, should never be visited again (to prevent recursion).
		*/
//		const TSet<const UObject*>& GetClosedObjects() const { return ClosedObjects; }

//		void CloseObject(const UObject* const InObject, bool* const pbOutAlreadyClosed = nullptr) 
//		{ 
//			check(InObject);
//			ClosedObjects.Add(InObject, pbOutAlreadyClosed);
//		}
//
//		bool AddField(const IMySaveable* const InField) 
//		{ 
//			return SL_NOT_IMPL_RET(true, TEXT("Write it!"));
//		}

//	private:		
//		TArray<const UField*> Fields;
//		TSet<const UObject*> ClosedObjects;
//	};

//	bool AreAllFieldsValid_AndAddNewFieldsToBeChecked(TScriptInterface<IMySaveable> InSaveable, FFieldVisitState& State, ELogFlags InLogFlags)
//	{
//		return SL_NOT_IMPL_RET(true, TEXT("Write it!"));
//	}
} // anonymous

bool UMySaveableUtils::AreAllFieldsValidRecursive(TScriptInterface<IMySaveable> InSaveable, ELogFlags InLogFlags)
{
//	FFieldVisitState State;
//
//	if( false == CheckAllFieldsValid_AndAddNewFieldsToBeChecked(InSaveable, State, InLogFlags) )
//	{
//		return false;
//	}
//
//	for(const UField* Field : State.GetFields())
//	{
//		SL_NOT_IMPL(TEXT("Case UField is UObject field"));
//
//		SL_NOT_IMPL(TEXT("Case UField is struct field"));
//	}
//
	// @TODO: Check SL_NOT_IMPL_RET InCore;
	SL_TO_BE_IMPL(TEXT("Write it"));
	return false;
}