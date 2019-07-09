#pragma once

#include "Util/TestUtil/TUActor.h"
#include "SaveLoad/IMySaveable.h"
#include "TestSaveable.generated.h"

UCLASS()
class ATestSaveable 
: public ATUActor
, public IMySaveable
{
	GENERATED_BODY()

public:
	ATestSaveable();

	// ~IMySaveable Begin
	virtual TScriptInterface<IMySaveableHandle> SaveLoad_GetHandle() const override;
	// ~IMySaveable End
};