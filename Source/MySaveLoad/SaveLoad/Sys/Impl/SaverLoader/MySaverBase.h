#pragma once

#include "MySaverLoaderBase.h"

/**
* Saves objects of world into the archive.
*
* Separate instance must be created for each save.
**/

#include "MySaverBase.generated.h"

class UMySaveState;
class IMySaveableHandle;

UCLASS()
class UMySaverBase :  public UMySaverLoaderBase
{
	GENERATED_BODY()

public:
	void SetupSaverBase
	(
		IMySaveLoadSystemInternal* InSys, 
		FArchive* InArchive, UWorld* InWorld, 
		UMySaveLoadState* InState
	);

	/**
	* The main function: saves world into the State!
	*/
	virtual void Save();

	/**
	* Extracts world info from world.
	*/
	void Extract_WorldInfo();

	/**
	* Finds all global objects.
	*
	* Typically to be implemented by subclass, adding global objects.
	*
	* WARNING! Super must always be called.
	*/
	virtual void Find_GlobalObjects();

	/**
	* Finds all saveable objects in the world.
	*
	* WARNING!!! Global objects must already be found, to separate world objects from global objects!
	*/
	void Find_WorldObjects();

	/**
	* Extracts class table based on the list of objects to save.
	*/
	void Extract_ClassTable();

	/**
	* Extracts destructed objects.
	*/
	void Extract_DestructedObjects();

protected:
	/**
	* Typically called within Find_GlobalObjects.
	* @param InObject - any object (supporting or non-supporing IMySaveable), maybe nullptr
	*/
	void RegisterGlobalObject_IfShouldBeSaved(UObject* InObject);

	void Find_GlobalObject_PlayerController();
	void Find_GlobalObject_GameMode();
	void Find_GlobalObject_GameInstance();

private:
	void RegisterGlobalObject(TScriptInterface<IMySaveableHandle> const InSaveableHandle);
	void BindClassIndicesToObjects(const TArray<TScriptInterface<IMySaveableHandle>>& InSaveableHandles);

	/**
	* Entire state is converted into binary representation and saved to archive.
	*/
	void BinarizeToArchive();
};