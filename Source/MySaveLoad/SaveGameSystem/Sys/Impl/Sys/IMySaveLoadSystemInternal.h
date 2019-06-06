#pragma once

/**
* Interface for doing internal calls from the save system itself.
*/

#include "SaveGameSystem/IMySaveLoadSystem.h"
#include "IMySaveLoadSystemInternal.generated.h"

class IMySaveable;

UINTERFACE()
class UMySaveLoadSystemInternal : public UMySaveLoadSystem
{
	GENERATED_BODY()
};

class IMySaveLoadSystemInternal : public IMySaveLoadSystem
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