#pragma once

/**
* Base class of both saver and loader.
*/

#include "../Format/MySaveTypes.h"
#include "../Sys/IMySaveSystemInternal.h"
#include "MySaverLoaderBase.generated.h"

class UMySaveLoadState;
class UMySaveState;
class UPerObjectSaveLoadDataBase;

class IMySaveable;
class IMySaveSystemInternal;

class FArchive;

UENUM()
enum class ESaverOrLoader : uint8
{
	Saver = 0,
	Loader = 1
};

UCLASS()
class UMySaverLoaderBase : 
	public UObject
{
	GENERATED_BODY()

public:
	void SetupSaverLoaderBase
	(
		ESaverOrLoader InSaverOrLoader,
		IMySaveSystemInternal* InSys,
		FArchive* InArchive, UWorld* InWorld,
		UMySaveLoadState* InCommState
	);

	IMySaveSystemInternal* GetSys() const { return Sys; }

	FArchive& GetAr() const { return *Ar; }
	virtual UWorld* GetWorld() const override { return World; }
	FMySavedWorld& GetBinaryWorld() { return BinaryWorld; }
	const FMySavedWorld& GetBinaryWorld() const { return BinaryWorld; }
	UMySaveLoadState* GetCommState() const { return CommState; }
	TSubclassOf<UPerObjectSaveLoadDataBase> GetPerObjectDataClass() const { return PerObjectDataClass; }

	// ~Helpers Begin
	/**
	* Checks whether the given object is global based on the state!
	*/
	bool IsGlobalObject(UObject* InObject) const;

	/**
	* Checks whether the given object be saved at all!
	*/
	bool ShouldObjectBeSaved(UObject* InObj, bool bInLogged=false, bool bLogOnFalseOnly=false) const;
	// ~Helpers End

protected:
	/**
	* Performs serialization/deserialization of the world to/from archive.
	*/
	void SerializeToFromArchive();

	/**
	* Logs binary world state
	*/
	void LogBinaryWorld();

	/**
	* Assigns data to all objects (both world and global)
	* Warning both world and global data objects already must be created in the state object!
	*/
	void AssignDataToAllObjects();
	void AssignObjectData(TScriptInterface<IMySaveable> InObj);

private:
	IMySaveSystemInternal* Sys = nullptr;

	FArchive* Ar = nullptr;

	UPROPERTY()
	UWorld* World = nullptr;

	// ~Per-class Props Begin
	UPROPERTY()	
	ESaverOrLoader SaverOrLoader;

	UPROPERTY()
	TSubclassOf<UPerObjectSaveLoadDataBase> PerObjectDataClass = nullptr;
	// ~Per-class Props End

	UPROPERTY()
	UMySaveLoadState* CommState = nullptr;

	UPROPERTY()
	FMySavedWorld BinaryWorld;
};