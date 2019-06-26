#pragma once

#include "Misc/AutomationTest.h"

class FSaveLoadTestBase : public FAutomationTest
{
public:
	FSaveLoadTestBase(const FString& InName, bool const bInComplexTask);

private:
	// @TODO: Save load system
};