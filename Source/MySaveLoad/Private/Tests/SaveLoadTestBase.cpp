#include "SaveLoadTestBase.h"
#include "SaveLoad/Util/SaveLoadLogUtilLib.h"
#include "SaveLoad/Sys/MySaveLoadSystemQuick.h"
#include "SaveLoad/MySaveLoadLib.h"
#include "Util/Core/WorldUtilLib.h"

#include "Util/TestUtil/TUActor.h"

#include "Engine/World.h"

/**
* TODO 
*
* TODO Spawn:
* 1. Fix parameters: should take NAME
*
* TODO SaveLoad
*/

const FString FSaveLoadTestBase::SAVE_SLOT_NAME { FString(TEXT("TEST_SLOT")) };
const int32 FSaveLoadTestBase::SAVE_USER_INDEX = 0;

FSaveLoadTestBase::FSaveLoadTestBase(const FString& InName, bool const bInComplexTask)
:	FAutomationTestBase{InName, bInComplexTask}
{
	SL_LOGFUNC_MSG(TEXT("CTOR for TEST named \"%s\""), *InName);
}
FSaveLoadTestBase::~FSaveLoadTestBase()
{
	SL_LOGFUNC();

	if(UObject* const SysObj = GetSysObj())
	{
		M_LOGBLOCK(TEXT("Removing system object from root (on valid pointer)"));
		SysObj->RemoveFromRoot();
	}

	UWorldUtilLib::DestroyWorldSafe(&World);
}

bool FSaveLoadTestBase::RunTest(const FString& Parameters)
{
	SL_LOGFUNC_MSG(TEXT("Running test"));

	if( ! InitializeWorld() )
	{
		return false;
	}

	if( ! InitializeSaveLoadSystem() )
	{
		return false;
	}

	// @note: typically prepared world actors are save load system actors,
	// so the save load system must be already up-and-running
	if( ! PrepareWorldLogged (World, Sys, Parameters) )
	{
		return false;
	}

	bool bSucceeded;
	{
		SL_LOGBLOCK(TEXT("*** TEST \"%s\": REAL TEST CODE ***"), *GetTestName());
		bSucceeded = MyRunTest(Parameters);
	}
	return bSucceeded;
}


void FSaveLoadTestBase::SaveLoadChecked()
{
	bool const bSucceeded = SaveLoad();
	TestTrue(TEXT("Checked save-load must succeed"), bSucceeded);
}

bool FSaveLoadTestBase::SaveLoad()
{
	M_LOGFUNC();
	checkNoRecursion();
	if( ! Save() )
	{
		return false;
	}
	if( ! Load() )
	{
		return false;
	}
	return true;
}

bool FSaveLoadTestBase::Save()
{
	M_LOGFUNC();
	bool const bSucceeded = UMySaveLoadLib::Save(GetSysObj(), SAVE_SLOT_NAME, SAVE_USER_INDEX);
	return bSucceeded;
}

bool FSaveLoadTestBase::Load()
{
	M_LOGFUNC();
	bool const bSucceeded = UMySaveLoadLib::Load(GetSysObj(), SAVE_SLOT_NAME, SAVE_USER_INDEX);
	return bSucceeded;
}

bool FSaveLoadTestBase::PrepareWorldLogged(UWorld* const InWorld, IMySaveLoadSystem* const InSys, const FString& InParameters)
{
	SL_LOGFUNC_MSG(TEXT("*** PREPARE WORLD for test \"%s\""), *GetTestName());

	checkf(InWorld, TEXT("Before calling %s UWorld always must be initialized"), TEXT(__FUNCTION__));
	checkf(InSys, TEXT("Before calling %s Save load system always must be initialized because SaveLoad objects're often spawned into the world"), TEXT(__FUNCTION__));

	if( ! PrepareWorld(InWorld, InSys, InParameters) )
	{
		M_LOG_ERROR(TEXT("Prepare world failed"));
		return false;
	}

	return true;
}

bool FSaveLoadTestBase::InitializeSaveLoadSystem()
{
	SL_LOGFUNC();
	
	checkf(World, TEXT("Before calling %s UWorld always must be initialized"), TEXT(__FUNCTION__));

	FQuickSaveLoadSystemInitializer SystemCfg { World };
	Sys = UMySaveLoadSystemQuick::NewQuickSaveLoadSystem(Cast<UObject>(GetTransientPackage()), SystemCfg);
	if(Sys == nullptr)
	{
		M_LOG_ERROR( TEXT("Save load system initialization failed") );
		return false;
	}

	GetSysObj()->AddToRoot();

	return true;
}
bool FSaveLoadTestBase::InitializeWorld()
{
	SL_LOGFUNC();

	World = UWorldUtilLib::NewWorldAndContext();
	if(World == nullptr)
	{
		M_LOG_ERROR( TEXT("Initialize world failed") );
		return false;
	}

	return true;
}

UObject* FSaveLoadTestBase::GetSysObj() const
{
	return Cast<UObject>(Sys);
}

AActor* FSaveLoadTestBase::Spawn(UClass* InClass, const FVector& InLocation, const FRotator& InRotation)
{
	return UWorldUtilLib::Spawn(World, InClass, InLocation, InRotation);
}

ATUActor* FSaveLoadTestBase::SpawnTU(const FVector& InLocation, const FRotator& InRotation)
{
	return Spawn<ATUActor>(InLocation, InRotation);
}
