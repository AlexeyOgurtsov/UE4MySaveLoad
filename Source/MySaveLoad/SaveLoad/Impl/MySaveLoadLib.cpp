#include "SaveLoad/MySaveLoadLib.h"
#include "SaveLoad/Util/SaveLoadLogUtilLib.h"
#include "SaveLoad/IMySaveLoadSystem.h"
#include "SaveLoad/Util/MySaveArchive.h"
#include "SaveLoad/MySaveGame.h"

#include "Kismet/GameplayStatics.h"

/**
* TODO:
* 1. Create save game
* 1.1. Create save game class
* 2. Create archive
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

	MY_SAVE_GAME_CLASS_MACRO* const S = Cast<MY_SAVE_GAME_CLASS_MACRO>(UGameplayStatics::CreateSaveGameObject(FImplUtils::SaveGameClass));
	if(S == nullptr)
	{
		M_LOG_ERROR_IF_FLAGS(InLogFlags, TEXT("UGameplayStatics::CreateSaveGameObject returned nullptr"));
		return false;
	}

	M_NOT_IMPL();
	return true;
}

bool UMySaveLoadLib::Load(TScriptInterface<IMySaveLoadSystem> const InSys, const FString& InSlotName, int32 const InUserIndex, ELogFlags InLogFlags)
{
	SL_LOGFUNC_IF_FLAGS(InLogFlags);

	checkf(InSys.GetInterface(), TEXT("%s requires non-NULL system to be passed"), TEXT(__FUNCTION__)); 

	M_NOT_IMPL();
	return true;
}
