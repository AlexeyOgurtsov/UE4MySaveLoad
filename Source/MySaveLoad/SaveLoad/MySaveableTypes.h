#pragma once

#include "MySaveableTypes.generated.h"

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
