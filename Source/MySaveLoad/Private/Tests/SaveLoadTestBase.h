#pragma once

#include "Misc/AutomationTest.h"
#include "SaveLoad/Sys/MySaveLoadSystemQuick.h"
#include "Templates/UniquePtr.h"

class IMySaveLoadSystem;

class FSaveLoadTestBase : public FAutomationTestBase
{
public:
	FSaveLoadTestBase(const FString& InName, bool const bInComplexTask);

	IMySaveLoadSystem* GetSys() const { return Sys.Get(); }

protected:
	/**
	* Constructs the save load system and initializes it, so the calls can be made on it.
	*/
	void InitializeSaveLoadSystem();

private:
	TUniquePtr<IMySaveLoadSystem> Sys;
};