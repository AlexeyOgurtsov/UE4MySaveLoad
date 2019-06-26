#include "SaveLoadTestBase.h"
#include "SaveLoad/Util/SaveLoadLogUtilLib.h"
#include "SaveLoad/Sys/SaveLoadSystemQuick.h"

FSaveLoadTestBase::FSaveLoadTestBase(const FString& InName, bool const bInComplexTask)
:	FAutomationTest{InName, bInComplexTask}
{
	SL_LOGFUNC_MSG(TEXT("CTOR for TEST named \"%s\""), *InName);
}

void FSaveLoadTestBase::InitializeSaveLoadSystem()
{
	SL_LOGFUNC();

	FQuickSaveLoadSystemInitializer SystemCfg;
	Sys.Reset(UQuickSaveLoadSystem::NewQuickSaveLoadSystem(Cast<UObject>(GetTransientPackage()), SystemCfg));
	checkf(Sys.Get(), TEXT("Save load system creation method should return valid pointer"));
}