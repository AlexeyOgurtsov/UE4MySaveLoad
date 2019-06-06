#include "SaveGameSystem/IMySaveable.h"
#include "Util/Core/LogUtilLib.h"

FString IMySaveable::SaveLoad_ToString() const
{
	const UObject* const ThisObj = Cast<UObject>(this);
	TArray<FStringFormatArg> FormatArgs;
	FormatArgs.Add(GetUniqueName());
	FormatArgs.Add(ThisObj->GetName());
	FormatArgs.Add(ThisObj->GetClass()->GetName());
	return FString::Format(TEXT("{UniqueName=\"{0}\"; Name=\"{1}\"; Class=\"{2}\"}"), FormatArgs);
}

FString IMySaveable::SaveLoad_ToStringPrefixed(const FString& InPrefix)
{
	FString Result = InPrefix;
	Result.Append(TEXT(": "));
	Result.Append(SaveLoad_ToString());
	return Result;
}

FString IMySaveable::SaveLoad_GetPrefixString(const FString& InPrefix)
{
	FString Result = InPrefix;
	Result.Append(TEXT(" for "));
	Result.Append(SaveLoad_ToString());
	Result.Append(TEXT(":"));
	return Result;
}

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