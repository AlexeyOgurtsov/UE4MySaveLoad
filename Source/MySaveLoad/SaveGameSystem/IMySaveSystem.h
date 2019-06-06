#pragma once

/**
* Public interface of the save system (for calls from IMySaveable, for example).
*/
#include "IMySaveSystem.generated.h"

class IMySaveable;
class IMySaveableHandle;

UINTERFACE()
class UMySaveSystem : public UInterface
{
	GENERATED_BODY()
};


class IMySaveSystem
{
	GENERATED_BODY()

public:
	virtual TScriptInterface<IMySaveableHandle> CreateSaveableHandle(TScriptInterface<IMySaveable> InSaveable) = 0;
	virtual void NotifyObjectDestructed(TScriptInterface<IMySaveableHandle> InSaveableHandle) = 0;
};



