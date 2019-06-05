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

	// ~Validation Begin
	/**
	* Returns true if all flags are set as valid.
	*/
	static bool IsSaveableValid(TScriptInterface<IMySaveable> InObj, bool bInLogged = true);
	static bool AreSaveableFlagsValid(TScriptInterface<IMySaveable> InObj, bool bInLogged = true);
	// ~Validation End
};



