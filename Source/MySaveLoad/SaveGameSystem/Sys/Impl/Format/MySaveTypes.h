#pragma once

#include "MySaveTypes.generated.h"

/**
* Info about format itself.
*/
USTRUCT()
struct FMySaveFormatMeta
{
	GENERATED_BODY()

	/**
	* Signature of the format.
	*/
	UPROPERTY()
	FString Signature;
};

inline FArchive& operator<<(FArchive& Ar, FMySaveFormatMeta& InMeta)
{
	Ar << InMeta.Signature;
	return Ar;
}


/**
* Info about world itself.
*/
USTRUCT()
struct FMySavedWorldInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FString MapName;
};

inline FArchive& operator<<(FArchive& Ar, FMySavedWorldInfo& InWorld)
{
	Ar << InWorld.MapName;
	return Ar;
}

/**
* Saved destroyed object.
*/
USTRUCT()
struct FMySavedDestruct
{
	GENERATED_BODY()

	UPROPERTY()
	FString UniqueName;
};
inline FArchive& operator<<(FArchive& Ar, FMySavedDestruct& InObj)
{
	Ar << InObj.UniqueName;
	return Ar;
}

/**
* Saved object or actor.
*/
USTRUCT()
struct FMySavedObject
{
	GENERATED_BODY()

	UPROPERTY()
	FString UniqueName;

	UPROPERTY()
	int32 ClassIndex = -1;

	UPROPERTY()
	TArray<uint8> Data;
};

inline FArchive& operator<<(FArchive& Ar, FMySavedObject& InObj)
{
	Ar << InObj.UniqueName << InObj.ClassIndex;
	Ar << InObj.Data;
	return Ar;
}

USTRUCT()
struct FMySavedClass
{
	GENERATED_BODY()

	UPROPERTY()
	FString Name;
};


inline FArchive& operator<<(FArchive& Ar, FMySavedClass& InClass)
{
	Ar << InClass.Name;
	return Ar;
}

/**
* Entire saved level.
*/
USTRUCT()
struct FMySavedWorld
{
	GENERATED_BODY()

	UPROPERTY()
	FMySaveFormatMeta FormatMeta;

	UPROPERTY()
	FMySavedWorldInfo WorldInfo;

	/**
	* All used classes.
	*/
	UPROPERTY()
	TArray<FMySavedClass> Classes;

	/**
	* All existing global objects (game instance etc.)
	*/
	UPROPERTY()
	TArray<FMySavedObject> GlobalObjects;

	/**
	* All existing objects in the world (excluding global)
	*/
	UPROPERTY()
	TArray<FMySavedObject> WorldObjects;

	/**
	* List of objects that existed initially, but were destroyed.
	*/
	UPROPERTY()
	TArray<FMySavedDestruct> DestructedObjects;
};

inline FArchive& operator<<(FArchive& Ar, FMySavedWorld& InWorld)
{
	Ar << InWorld.FormatMeta;
	Ar << InWorld.WorldInfo;
	Ar << InWorld.Classes;
	Ar << InWorld.GlobalObjects;
	Ar << InWorld.WorldObjects;
	Ar << InWorld.DestructedObjects;
	return Ar;
}

UCLASS()
class UMySavedTypes : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
};