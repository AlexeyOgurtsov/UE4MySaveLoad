#include "SaveLoadTestBase.h"
#include "SaveLoad/Util/SaveLoadLogUtilLib.h"

#include "Util/TestUtil/TUActor.h"

/**
* TODO: Save/Load test.
* 1. Check actors loaded after save
*/

#define SAVELOAD_TRIVIAL_TEST_PREFIX "SaveLoad.Trivial."

IMPLEMENT_SAVELOAD_TEST(FSaveLoad_Trivial_First, SAVELOAD_TRIVIAL_TEST_PREFIX "First", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);
IMPLEMENT_SAVELOAD_TEST(FSaveLoad_Trivial_Second, SAVELOAD_TRIVIAL_TEST_PREFIX "Second", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);

// ~First trivial test Begin
bool FSaveLoad_Trivial_First::MyRunTest(const FString& Parameters)
{
	SaveLoad();
	return true;
};

bool FSaveLoad_Trivial_First::PrepareWorld(UWorld* InWorld, IMySaveLoadSystem* InSys, const FString& Parameters)
{
	ATUActor* const A = SpawnTU(FVector{0,0,0});
	ATUActor* const A2 = SpawnTU(FVector{1.0F, 0.0F, 0.0F});

	return true;
}
// ~First trivial test End

// ~Second trivial test Begin
bool FSaveLoad_Trivial_Second::MyRunTest(const FString& Parameters)
{
	return true;
};

bool FSaveLoad_Trivial_Second::PrepareWorld(UWorld* InWorld, IMySaveLoadSystem* InSys, const FString& Parameters)
{
	return true;
}
// ~Second trivial test End
