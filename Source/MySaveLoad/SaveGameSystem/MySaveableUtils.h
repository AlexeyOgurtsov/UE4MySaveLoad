#pragma once

/**
* Utils to be used within the IMySaveable implementation.
*/

#include "MySaveableUtils.generated.h"

class IMySaveable;
class FArchive;

UCLASS()
class UMySaveableUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// ~Implementation helpers Begin
	static void SerializeObjectData(FArchive& Ar, UObject* InObj);
	// ~Implementation helpers End

	// ~Helper getters Begin
	static const FName& GetUniqueName(TScriptInterface<IMySaveable> InObj);

	static bool IsGlobal(TScriptInterface<IMySaveable> InObj);
	static bool IsSaveLoad(TScriptInterface<IMySaveable> InObj);
	static bool IsDynamic(TScriptInterface<IMySaveable> InObj);
	static bool IsStatic(TScriptInterface<IMySaveable> InObj);
	// ~Helper getters End

	// ~Validation Begin
	/**
	* Returns true if all flags are set as valid.
	*/
	static bool IsSaveableValid(TScriptInterface<IMySaveable> InObj, bool bInLogged = true);
	static bool AreSaveableFlagsValid(TScriptInterface<IMySaveable> InObj, bool bInLogged = true);
	// ~Validation End
};



