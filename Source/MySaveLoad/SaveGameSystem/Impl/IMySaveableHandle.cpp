#include "SaveGameSystem/IMySaveableHandle.h"
#include "Util/Core/LogUtilLib.h"

FString IMySaveableHandle::SaveLoad_ToString() const
{
	const UObject* const ThisObj = Cast<UObject>(this);
	TArray<FStringFormatArg> FormatArgs;
	FormatArgs.Add(SaveLoad_GetUniqueName());
	FormatArgs.Add(ThisObj->GetName());
	FormatArgs.Add(ThisObj->GetClass()->GetName());
	return FString::Format(TEXT("{UniqueName=\"{0}\"; Name=\"{1}\"; Class=\"{2}\"}"), FormatArgs);
}

FString IMySaveableHandle::SaveLoad_ToStringPrefixed(const FString& InPrefix)
{
	FString Result = InPrefix;
	Result.Append(TEXT(": "));
	Result.Append(SaveLoad_ToString());
	return Result;
}

FString IMySaveableHandle::SaveLoad_GetPrefixString(const FString& InPrefix)
{
	FString Result = InPrefix;
	Result.Append(TEXT(" for "));
	Result.Append(SaveLoad_ToString());
	Result.Append(TEXT(":"));
	return Result;
}

FString IMySaveableHandle::SaveLoad_GetUniqueName() const
{
	return SaveLoad_GetUniqueFName().ToString();
}

const FName& IMySaveableHandle::SaveLoad_GetUniqueFName() const
{
	const FMySaveableStaticProps& Props = SaveLoad_GetStaticProps();
	return Props.UniqueName;
}

bool IMySaveableHandle::SaveLoad_IsEnabled() const
{
	const FMySaveableStaticProps& Props = SaveLoad_GetStaticProps();
	return (Props.Flags & EMySaveableStaticFlags::SaveLoad) != EMySaveableStaticFlags::None;
}

bool IMySaveableHandle::SaveLoad_IsGlobal() const
{
	const FMySaveablePerClassProps& Props = SaveLoad_GetClassProps();
	return (Props.Flags & EMySaveablePerClassFlags::GlobalObject) != EMySaveablePerClassFlags::None;
}

bool IMySaveableHandle::SaveLoad_IsDynamic() const
{
	const FMySaveableStaticProps& Props = SaveLoad_GetStaticProps();
	return (Props.Flags & EMySaveableStaticFlags::Dynamic) != EMySaveableStaticFlags::None;
}

bool IMySaveableHandle::SaveLoad_IsStatic() const
{
	return false == SaveLoad_IsDynamic();
}