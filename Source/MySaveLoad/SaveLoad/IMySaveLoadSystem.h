#pragma once

/**
* Public interface of the save system (for calls from IMySaveable, for example).
*/
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
	virtual TScriptInterface<IMySaveableHandle> CreateSaveableHandle(TScriptInterface<IMySaveable> InSaveable) = 0;
	virtual void NotifyObjectDestructed(TScriptInterface<IMySaveableHandle> InSaveableHandle) = 0;
};



