#include "SaveLoad/IMySaveable.h"
#include "SaveLoad/IMySaveableHandle.h"
#include "SaveLoad/MySaveableUtils.h"

void IMySaveable::SaveLoad_Serialize(FArchive& Ar)
{
	return UMySaveableUtils::Default_Serialize(Cast<UObject>(this), Ar);
}

void IMySaveable::SaveLoad_BeforeDestroy()
{
	return UMySaveableUtils::Default_BeforeDestroy(Cast<UObject>(this));
}

void IMySaveable::SaveLoad_AllObjectsLoaded(FArchive& Ar)
{
	return UMySaveableUtils::Default_AllObjectsLoaded(Cast<UObject>(this), Ar);
}

FString IMySaveable::SaveLoad_ToString() const
{
	return SaveLoad_GetHandle()->SaveLoad_ToString();
}

FString IMySaveable::SaveLoad_ToStringPrefixed(const FString& InPrefix)
{
	return SaveLoad_GetHandle()->SaveLoad_ToStringPrefixed(InPrefix);
}

FString IMySaveable::SaveLoad_GetPrefixString(const FString& InPrefix)
{
	return SaveLoad_GetHandle()->SaveLoad_GetPrefixString(InPrefix);
}

FString IMySaveable::SaveLoad_GetUniqueName() const
{
	return SaveLoad_GetHandle()->SaveLoad_GetUniqueName();
}

const FName& IMySaveable::SaveLoad_GetUniqueFName() const
{
	return SaveLoad_GetHandle()->SaveLoad_GetUniqueFName();
}

bool IMySaveable::SaveLoad_IsGlobal() const
{
	return SaveLoad_GetHandle()->SaveLoad_IsGlobal();
}

bool IMySaveable::SaveLoad_IsEnabled() const
{
	return SaveLoad_GetHandle()->SaveLoad_IsEnabled();
}

bool IMySaveable::SaveLoad_IsDynamic() const
{
	return SaveLoad_GetHandle()->SaveLoad_IsDynamic();
}

bool IMySaveable::SaveLoad_IsStatic() const
{
	return SaveLoad_GetHandle()->SaveLoad_IsStatic();
}

const FMySaveablePerClassProps& IMySaveable::SaveLoad_GetClassProps() const
{
	return SaveLoad_GetHandle()->SaveLoad_GetClassProps();
}

const FMySaveableStaticProps& IMySaveable::SaveLoad_GetStaticProps() const
{
	return SaveLoad_GetHandle()->SaveLoad_GetStaticProps();
}