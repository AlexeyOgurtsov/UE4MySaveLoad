#pragma once

/**
* Public interface of the save system (for calls from IMySaveable, for example).
*/
#include "IMySaveSystem.generated.h"

class IMySaveable;

UINTERFACE()
class UMySaveSystem : public UInterface
{
	GENERATED_BODY()
};


class IMySaveSystem
{
	GENERATED_BODY()

public:
	virtual void NotifyObjectDestructed(TScriptInterface<IMySaveable> InObject) = 0;
};



