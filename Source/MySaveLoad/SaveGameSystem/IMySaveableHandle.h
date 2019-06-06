#pragma once

/**
* Handle that each saveable object instance uses to interact with the save-load system.
*/

#include "SaveGameSystem/MySaveableTypes.h"
#include "IMySaveableHandle.generated.h"

class IMySaveable;
class UMySaverLoaderBase;
class UPerObjectSaveLoadDataBase;

UINTERFACE()
class UMySaveableHandle : public UInterface
{
	GENERATED_BODY()
};

class IMySaveableHandle
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

	FString SaveLoad_ToString() const;

	/**
	* Returns string in form "{Prefix}: {SaveLoad_ToString() result}"
	*/
	FString SaveLoad_ToStringPrefixed(const FString& InPrefix);

	/**
	* Returns string formatted like a prefix string (for function calls, for example).
	*/
	FString SaveLoad_GetPrefixString(const FString& InPrefix);
	// ~Helper getters End

	virtual TScriptInterface<IMySaveable> SaveLoad_GetSaveable() const = 0;

	virtual const FMySaveablePerClassProps& SaveLoad_GetClassProps() const = 0;
	virtual const FMySaveableStaticProps& SaveLoad_GetStaticProps() const = 0;

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
};