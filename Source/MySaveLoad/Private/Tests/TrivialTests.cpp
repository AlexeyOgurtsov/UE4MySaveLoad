#include "SaveLoadTestBase.h"
#include "SaveLoad/Util/SaveLoadLogUtilLib.h"

#define SAVELOAD_TRIVIAL_TEST_PREFIX "SaveLoad.Trivial."

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FSaveLoad_Trivial_First, FSaveLoadTestBase, SAVELOAD_TRIVIAL_TEST_PREFIX "First", EAutomationTestFlags::EditorContext | EAutomationTestFlags::SmokeFilter);

bool FSaveLoad_Trivial_First::RunTest(const FString& Parameters)
{
	SL_LOGFUNC();

	return true;
}