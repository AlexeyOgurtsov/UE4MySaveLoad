#pragma once

/**
* Handle that each saveable object instance uses to interact with the save-load system.
*/

#include "IMySaveableHandle.generated.h"

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