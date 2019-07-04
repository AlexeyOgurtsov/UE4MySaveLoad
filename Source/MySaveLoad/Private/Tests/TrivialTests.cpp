#include "SaveLoadTestBase.h"
#include "SaveLoad/Util/SaveLoadLogUtilLib.h"

#define SAVELOAD_TRIVIAL_TEST_PREFIX "SaveLoad.Trivial."

IMPLEMENT_SAVELOAD_TEST(FSaveLoad_Trivial_First, SAVELOAD_TRIVIAL_TEST_PREFIX "First", EAutomationTestFlags::EditorContext | EAutomationTestFlags::SmokeFilter);

bool FSaveLoad_Trivial_First::MyRunTest(const FString& Parameters)
{
	return true;
};
