#include "SaveLoadTestBase.h"
#include "SaveLoad/Util/SaveLoadLogUtilLib.h"
#include "SaveLoad/Sys/MySaveLoadSystemQuick.h"
#include "Util/Core/WorldUtilLib.h"

#include "Engine/World.h"

/**
* TODO
*/

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

	if(World)
	{
		M_LOGBLOCK(TEXT("UWorld::DestroyWorld (on valid pointer)"));
		World->DestroyWorld(/*bInformWorld*/true, /*NewWorld*/nullptr);
	}
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

	bool bSucceeded;
	{
		SL_LOGBLOCK(TEXT("*** TEST \"%s\": REAL TEST CODE ***"), *GetTestName());
		bSucceeded = MyRunTest(Parameters);
	}
	return bSucceeded;
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
