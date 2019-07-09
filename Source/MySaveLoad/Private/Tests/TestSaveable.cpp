#include "TestSaveable.h"
#include "Util/Core/LogUtilLib.h"

ATestSaveable::ATestSaveable()
{
	// Create handle here
	M_NOT_IMPL_MSG(TEXT("Create SaveLoad handle here"));
}

TScriptInterface<IMySaveableHandle> ATestSaveable::SaveLoad_GetHandle() const
{
	M_NOT_IMPL_RET(TScriptInterface<IMySaveableHandle>(nullptr));
}
