#pragma once

/**
* Base class of both saver and loader.
*/

#include "../Format/MySaveTypes.h"
#include "../Sys/IMySaveLoadSystemInternal.h"
#include "Util/Core/Log/MyLoggingTypes.h"

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Templates/SubclassOf.h"
#include "MySaverLoaderBase.generated.h"

class UMySaveLoadState;
class UMySaveState;
class UPerObjectSaveLoadDataBase;

class IMySaveableHandle;
class IMySaveLoadSystemInternal;

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
		IMySaveLoadSystemInternal* InSys,
		FArchive* InArchive, UWorld* InWorld,
		UMySaveLoadState* InState
	);

	IMySaveLoadSystemInternal* GetSys() const { return Sys; }

	FArchive& GetAr() const { return *Ar; }
	virtual UWorld* GetWorld() const override { return World; }
	FMySavedWorld& GetBinaryWorld() { return BinaryWorld; }
	const FMySavedWorld& GetBinaryWorld() const { return BinaryWorld; }
	UMySaveLoadState* GetState() const { return State; }
	TSubclassOf<UPerObjectSaveLoadDataBase> GetPerObjectDataClass() const { return PerObjectDataClass; }

	// ~Helpers Begin
	/**
	* Checks whether the given object is global based on the state!
	*/
	bool IsGlobalObject(TScriptInterface<IMySaveableHandle> InSaveableHandle) const;

	/**
	* Checks whether the given object be saved at all!
	*/
	bool ShouldObjectBeSaved(TScriptInterface<IMySaveableHandle> InSaveableHandle, ELogFlags InLogFlags=ELogFlags::None) const;
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
	void AssignObjectData(TScriptInterface<IMySaveableHandle> InSaveableHandle);

private:
	IMySaveLoadSystemInternal* Sys = nullptr;

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
	UMySaveLoadState* State = nullptr;

	UPROPERTY()
	FMySavedWorld BinaryWorld;
};