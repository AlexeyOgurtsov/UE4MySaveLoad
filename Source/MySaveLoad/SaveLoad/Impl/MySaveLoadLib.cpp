#include "SaveLoad/MySaveLoadLib.h"
#include "SaveLoad/Util/SaveLoadLogUtilLib.h"
#include "SaveLoad/IMySaveLoadSystem.h"
#include "SaveLoad/Util/MySaveArchive.h"
#include "SaveLoad/MySaveGame.h"

#include "Kismet/GameplayStatics.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"

/**
* TODO:
*/

namespace
{
#define MY_SAVE_GAME_CLASS_MACRO UMySaveGame

	class FImplUtils
	{
	public:
		static UClass* const SaveGameClass;

	private:
	};
	UClass* const FImplUtils::SaveGameClass = MY_SAVE_GAME_CLASS_MACRO::StaticClass();

} // anonymous

bool UMySaveLoadLib::Save(TScriptInterface<IMySaveLoadSystem> const InSys, const FString& InSlotName, int32 const InUserIndex, ELogFlags InLogFlags)
{
	SL_LOGFUNC_IF_FLAGS(InLogFlags);

	checkf(InSys.GetInterface(), TEXT("%s requires non-NULL system to be passed"), TEXT(__FUNCTION__)); 

	SL_LOG_IF_FLAGS(InLogFlags, TEXT("Slot:\"%s\"; UserIndex: %d"), *InSlotName, InUserIndex);

	MY_SAVE_GAME_CLASS_MACRO* const S = Cast<MY_SAVE_GAME_CLASS_MACRO>(UGameplayStatics::CreateSaveGameObject(FImplUtils::SaveGameClass));
	if(S == nullptr)
	{
		SL_LOG_ERROR_IF_FLAGS(InLogFlags, TEXT("UGameplayStatics::CreateSaveGameObject returned nullptr"));
		return false;
	}

	FMemoryWriter BufferArchive{S->Data, /*bIsPersistent=*/true};
	FMySaveArchive Ar { BufferArchive };
		
	if( ! InSys->Save(Ar) )
	{
		SL_LOG_ERROR_IF_FLAGS(InLogFlags, TEXT("System save method returned false"));
		return false;
	}

	if ( ! UGameplayStatics::SaveGameToSlot(S, InSlotName, InUserIndex) )
	{
		SL_LOG_ERROR_IF_FLAGS(InLogFlags, TEXT("UGameplayStatics::SaveGameToSlot returned false"));
		return false;
	}

	return true;
}

bool UMySaveLoadLib::Load(TScriptInterface<IMySaveLoadSystem> const InSys, const FString& InSlotName, int32 const InUserIndex, ELogFlags InLogFlags)
{
	SL_LOGFUNC_IF_FLAGS(InLogFlags);

	checkf(InSys.GetInterface(), TEXT("%s requires non-NULL system to be passed"), TEXT(__FUNCTION__)); 

	SL_LOG_IF_FLAGS(InLogFlags, TEXT("Slot:\"%s\"; UserIndex: %d"), *InSlotName, InUserIndex);

	if( ! UGameplayStatics::DoesSaveGameExist(InSlotName, InUserIndex) )
	{
		SL_LOG_ERROR_IF_FLAGS(InLogFlags, TEXT("{Slot=\"%s\"; User=%d}: The given slot does NOT exist for the given user"), *InSlotName, InUserIndex);
		return false;
	}

	MY_SAVE_GAME_CLASS_MACRO* const S = Cast<MY_SAVE_GAME_CLASS_MACRO>(UGameplayStatics::LoadGameFromSlot(InSlotName, InUserIndex));
	if(S == nullptr)
	{
		SL_LOG_ERROR_IF_FLAGS(InLogFlags, TEXT("UGameplayStatics::LoadGameFromSlot returned nullptr"));
		return false;
	}

	FMemoryReader Reader { S->Data, /*bPersistent*/true };
	FMySaveArchive Ar { Reader };

	if( ! InSys->Load(Ar) )
	{
		SL_LOG_ERROR_IF_FLAGS(InLogFlags, TEXT("System load method returned false"));
		return false;
	}

	return true;
}
