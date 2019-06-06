#pragma once

#include "MySaveableTypes.h"
#include "IMySaveable.generated.h"

class UPerObjectSaveLoadDataBase;
class UMySaverLoaderBase;
class IMySaveableHandle;

class FArchive;

UINTERFACE()
class UMySaveable : public UInterface
{
	GENERATED_BODY()
};

class IMySaveable
{
	GENERATED_BODY()

public:
	// ~Helper getters Begin
	FString GetUniqueName() const;
	const FName& GetUniqueFName() const;

	bool IsGlobal() const;
	bool IsSaveLoad() const;
	bool IsDynamic() const;
	bool IsStatic() const;
	// ~Helper getters End

	virtual FString SaveLoad_ToString() const = 0;
	virtual TScriptInterface<IMySaveableHandle> SaveLoad_GetHandle() const = 0;

	/**
	* Returns string in form "{Prefix}: {SaveLoad_ToString() result}"
	*/
	FString SaveLoad_ToStringPrefixed(const FString& InPrefix);

	/**
	* Returns string formatted like a prefix string (for function calls, for example).
	*/
	FString SaveLoad_GetPrefixString(const FString& InPrefix);
	
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