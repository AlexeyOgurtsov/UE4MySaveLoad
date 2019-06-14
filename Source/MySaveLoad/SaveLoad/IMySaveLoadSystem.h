#pragma once

/**
* Public interface of the save system (for calls from IMySaveable, for example).
*/

#include "UObject/Interface.h"
#include "IMySaveLoadSystem.generated.h"

class IMySaveable;
class IMySaveableHandle;

UINTERFACE()
class UMySaveLoadSystem : public UInterface
{
	GENERATED_BODY()
};


class IMySaveLoadSystem
{
	GENERATED_BODY()

public:
	/**
	* Creates a new handle for the given saveable object, so ties it to the system.
	* @warning: must be called only from the constructor.
	*
	* @returns: always returns valid handle object.
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = SaveLoadSystem)
	TScriptInterface<IMySaveableHandle> CreateSaveableHandle(const TScriptInterface<IMySaveable>& InSaveable);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = SaveLoadSystem)
	void NotifySaveableDestructed(const TScriptInterface<IMySaveableHandle>& InSaveableHandle);
};



