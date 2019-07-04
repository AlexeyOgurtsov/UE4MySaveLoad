#include "SaveLoadTestBase.h"
#include "SaveLoad/Util/SaveLoadLogUtilLib.h"
#include "SaveLoad/Sys/MySaveLoadSystemQuick.h"

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
		SysObj->RemoveFromRoot();
	}
}

bool FSaveLoadTestBase::RunTest(const FString& Parameters)
{
	SL_LOGFUNC_MSG(TEXT("Running test"));
	
	InitializeSaveLoadSystem();

	bool bSucceeded;
	{
		SL_LOGBLOCK(TEXT("*** TEST \"%s\": REAL TEST CODE ***"), *GetTestName());
		bSucceeded = MyRunTest(Parameters);
	}
	return bSucceeded;
}

void FSaveLoadTestBase::InitializeSaveLoadSystem()
{
	SL_LOGFUNC();

	FQuickSaveLoadSystemInitializer SystemCfg;
	Sys = UMySaveLoadSystemQuick::NewQuickSaveLoadSystem(Cast<UObject>(GetTransientPackage()), SystemCfg);
	checkf(Sys, TEXT("Save load system creation method should return valid pointer"));
	GetSysObj()->AddToRoot();
}

UObject* FSaveLoadTestBase::GetSysObj() const
{
	return Cast<UObject>(Sys);
}
