#pragma once

/**
* Interface for doing internal calls from the save system itself.
*/

#include "SaveGameSystem/IMySaveSystem.h"
#include "IMySaveSystemInternal.generated.h"

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
	* Gets names of all destructed objects.
	*/
	virtual TArray<FName> GetDestructedObjects() const = 0;
};