#pragma once

#include "MySaverLoaderBase.h"

/**
* Loads objects from save-load-state into the world.
*
* Separate instance must be created for each load.
**/

#include "MyLoaderBase.generated.h"

class IMySaveableHandle;

UCLASS()
class UMyLoaderBase :  public UMySaverLoaderBase
{
	GENERATED_BODY()

public:
	void SetupLoaderBase
	(
		IMySaveLoadSystemInternal* InSys, 
		FArchive* InArchive, UWorld* InWorld, 
		UMySaveLoadState* InState
	);

	/**
	* The main function: loads state into the world!
	*/
	virtual void Load();

protected:
	void CheckAndLoadBinaryFromArchive();

	void LoadWorld();

	void LoadSavedClasses();
	void LoadSavedClass(int32 InClassIndex, UClass**ppOutClass, const FMySavedClass* SavedClass);

	/**
	* Destroy extra objects if they were marked as destroyed in the saved world.
	*/
	void DestroyExtraObjects();

	/**
	* Creates object into the object with the same name.
	* Creates object if it was created earlier.
	*
	* Warning: Never destroys objects (there's a separate function for that kind of stuff)
	*/
	TScriptInterface<IMySaveableHandle> LoadSavedObject(const FMySavedObject* pSavedObject);
};                                                    