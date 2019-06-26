#pragma once

#include "Misc/AutomationTest.h"
#include "Templates/UniquePtr.h"

class IMySaveLoadSystem;

class FSaveLoadTestBase : public FAutomationTest
{
public:
	FSaveLoadTestBase(const FString& InName, bool const bInComplexTask);

	IMySaveLoadSystem* GetSys() const { return Sys; }

protected:
	/**
	* Constructs the save load system and initializes it, so the calls can be made on it.
	*/
	void InitializeSaveLoadSystem();

private:
	TUniquePtr<IMySaveLoadSystem> Sys;
};