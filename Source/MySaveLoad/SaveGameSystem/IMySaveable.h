#pragma once

#include "IMySaveable.generated.h"

class FArchive;

class UPerObjectSaveLoadDataBase;
class UMySaverLoaderBase;

UENUM(BlueprintType, meta = (Bitflags))
enum class EMySaveablePerClassFlags : uint8
{
	None = 0x00,
	GlobalObject = 1 << 1, // Global-level object (PlayerController, GameInstance etc.)
};
ENUM_CLASS_FLAGS(EMySaveablePerClassFlags);

USTRUCT(BlueprintType, Category = SaveLoad)
struct FMySaveablePerClassProps
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = SaveLoad)
	EMySaveablePerClassFlags Flags;
};

UENUM(BlueprintType, meta = (Bitflags))
enum class EMySaveableStaticFlags : uint8
{
	None = 0x00,
	SaveLoad = 1 << 1, // The most important flag (!): is savegame enabled for this object at all?
	Dynamic = 1 << 2, // Whether this object was not loaded during level start, but was created dynamically?
};
ENUM_CLASS_FLAGS(EMySaveableStaticFlags);

/**
* Set of flags that can never change during lifetime of the object;
*/
USTRUCT(BlueprintType, Category = SaveLoad)
struct FMySaveableStaticProps
{
	GENERATED_BODY()

	/**
	* Unique name of the saveable object.
	* This name must be unique within ALL set of saveable objects!!!
	*
	* WARNING!!! Must always be set.
	*/
	FName UniqueName;

	/**
	* Warning this flags should NEVER change for the given game world!!!
	*/
	UPROPERTY(BlueprintReadOnly, Category = SaveLoad)
	EMySaveableStaticFlags Flags;
};

UINTERFACE()
class UMySaveable : public UInterface
{
	GENERATED_BODY()
};

class IMySaveable
{
	GENERATED_BODY()

public:
	/**
	* Per-Class props
	*/
	virtual const FMySaveablePerClassProps& SaveLoad_GetClassProps() const = 0;

	// ~SaveLoad data Begin
	/**
	* Assign data (to be called from loader only!).
	*/
	virtual void SaveLoad_AssignData(UMySaverLoaderBase* InSender, UPerObjectSaveLoadDataBase* Data) = 0;

	/**
	* Gets assigned loader data (to be called from loader only!).
	*/
	virtual UPerObjectSaveLoadDataBase* SaveLoad_GetData(UMySaverLoaderBase* InSender) const = 0;
	// ~SaveLoad data End

	virtual const FMySaveableStaticProps& SaveLoad_GetStaticProps() const = 0;
	virtual void SaveLoad_Serialize(FArchive& Ar) = 0;

	/**
	* Called when object is going to be destroyed because it was marked as destroyed here.
	* Typically implementing code here should manage links to the object here.
	*/
	virtual void SaveLoad_BeforeDestroy() = 0;

	/**
	* Called for all objects after all objects loaded,
	* but cross-object links are NOT fixed yet here.
	*
	* Typically here links between objects ared to be fixed;
	*/
	virtual void SaveLoad_AllObjectsLoaded(FArchive& Ar) = 0;
};