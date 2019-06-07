#pragma once

/**
* Data assigned for each object saveable during load.
*/

#include "SaveLoad/Base/PerObjectSaveLoadDataBase.h"
#include "PerObjectSaveLoadData.generated.h"

UCLASS()
class UPerObjectSaveLoadData : public UPerObjectSaveLoadDataBase
{
	GENERATED_BODY()

public:
	/**
	* Index of the class into the class table.
	*/
	UPROPERTY()
	int32 ClassIndex = 0;
};