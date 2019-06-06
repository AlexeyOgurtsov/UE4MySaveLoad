#pragma once

/**
* Interface for doing internal calls from the save system itself.
*/

#include "SaveGameSystem/IMySaveSystem.h"
#include "IMySaveSystemInternal.generated.h"

class IMySaveable;

UINTERFACE()
class UMySaveSystemInternal : public UMySaveSystem
{
	GENERATED_BODY()
};

class IMySaveSystemInternal : public IMySaveSystem
{
	GENERATED_BODY()

public:
	/**
	* Get all registered saveable objects.
	*/
	virtual const TArray<TScriptInterface<IMySaveable>>& GetSaveableObjects() const = 0;

	/**
	* Gets names of all static destructed objects.
	*/
	virtual const TArray<FName>& GetStaticDestructedObjects() const = 0;
};