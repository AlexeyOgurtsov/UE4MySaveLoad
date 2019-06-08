#pragma once

/**
* Interface for doing internal calls from the save system itself.
*/

#include "SaveLoad/IMySaveLoadSystem.h"
#include "IMySaveLoadSystemInternal.generated.h"

class IMySaveable;
class IMySaveableHandle;

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
	* Returns iterator that iterates all registered saveable object handles.
	* Iterates only objects with SaveLoad enabled.
	* @warning: Never delete saveable handles (and so, saveable objects) while iterating (as array is used)!
	*/
	virtual TArray<TScriptInterface<IMySaveableHandle>>::TConstIterator CreateSaveableHandleIterator() const =0;

	/**
	* Returns count of all registered saveable objects.
	*/
	virtual int32 NumSaveables() const = 0;

	/**
	* Gets unique names of all static destructed objects.
	*/
	virtual const TArray<FName>& GetStaticDestructedObjects() const = 0;
};