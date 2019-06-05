#include "SaveGameSystem/IMySaveable.h"
#include "Util/Core/LogUtilLib.h"

FString IMySaveable::GetUniqueName() const
{
	return GetUniqueFName().ToString();
}

const FName& IMySaveable::GetUniqueFName() const
{
	const FMySaveableStaticProps& Props = SaveLoad_GetStaticProps();
	return Props.UniqueName;
}

bool IMySaveable::IsGlobal() const
{
	const FMySaveablePerClassProps& Props = SaveLoad_GetClassProps();
	return (Props.Flags & EMySaveablePerClassFlags::GlobalObject) != EMySaveablePerClassFlags::None;
}

bool IMySaveable::IsSaveLoad() const
{
	const FMySaveableStaticProps& Props = SaveLoad_GetStaticProps();
	return (Props.Flags & EMySaveableStaticFlags::SaveLoad) != EMySaveableStaticFlags::None;
}

bool IMySaveable::IsDynamic() const
{
	const FMySaveableStaticProps& Props = SaveLoad_GetStaticProps();
	return (Props.Flags & EMySaveableStaticFlags::Dynamic) != EMySaveableStaticFlags::None;
}

bool IMySaveable::IsStatic() const
{
	return false == IsDynamic();
}