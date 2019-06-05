#pragma once

/**
* Quick implementation of the Save system.
*/

#include "SaveGameSystem/Sys/Impl/Sys/IMySaveSystemInternal.h"
#include "MySaveSystemQuick.generated.h"

UCLASS()
class UMySaveSystemQuick : 
	public UObject, 
	public IMySaveSystemInternal
{
	GENERATED_BODY()

public:
	/**
	* To be called for each destroyed IMySaveable object.
	*/
	virtual void NotifyObjectDestructed(TScriptInterface<IMySaveable> InObject) override;

	/**
	* Gets names of all destructed objects.
	*/
	virtual TArray<FName> GetStaticDestructedObjects() const override { return StaticDestructedObjects; }

private:
	UPROPERTY()
	TArray<FName> StaticDestructedObjects;
};