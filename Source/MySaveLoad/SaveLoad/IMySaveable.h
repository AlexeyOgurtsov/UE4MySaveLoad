#pragma once

#include "MySaveableTypes.h"
#include "UObject/Interface.h"
#include "IMySaveable.generated.h"

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
	FString SaveLoad_GetUniqueName() const;
	const FName& SaveLoad_GetUniqueFName() const;

	bool SaveLoad_IsEnabled() const;	
	bool SaveLoad_IsGlobal() const;
	bool SaveLoad_IsDynamic() const;
	bool SaveLoad_IsStatic() const;

	const FMySaveablePerClassProps& SaveLoad_GetClassProps() const;
	const FMySaveableStaticProps& SaveLoad_GetStaticProps() const;

	FString SaveLoad_ToString() const;

	const UObject* SaveLoad_GetObject() const { return Cast<const UObject>(this); }
	UObject* SaveLoad_GetObject() { return Cast<UObject>(this); }

	/**
	* Returns string in form "{Prefix}: {SaveLoad_ToString() result}"
	*/
	FString SaveLoad_ToStringPrefixed(const FString& InPrefix);

	/**
	* Returns string formatted like a prefix string (for function calls, for example).
	*/
	FString SaveLoad_GetPrefixString(const FString& InPrefix);
	// ~Helper getters End

	virtual TScriptInterface<IMySaveableHandle> SaveLoad_GetHandle() const = 0;
	
	virtual void SaveLoad_Serialize(FArchive& Ar);

	/**
	* Called when object is going to be destroyed because it was marked as destroyed here.
	* Typically implementing code here should manage links to the object here.
	*/
	virtual void SaveLoad_BeforeDestroy();

	/**
	* Called for all objects after all objects loaded,
	* but cross-object links are NOT fixed yet here.
	*
	* Typically here links between objects ared to be fixed;
	*/
	virtual void SaveLoad_AllObjectsLoaded(FArchive& Ar);
};