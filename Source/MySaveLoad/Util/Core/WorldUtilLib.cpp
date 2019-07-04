#include "WorldUtilLib.h"
#include "LogUtilLib.h"

#include "Engine/World.h"
#include "Engine/Engine.h"

UWorld* UWorldUtilLib::NewWorldAndContext
(
	EWorldType::Type const InWorldType,
	ENewWorldFlags const InFlags,
	FName const InWorldName,
	ELogFlags const InLogFlags
)
{
	M_LOGFUNC_IF_FLAGS(InLogFlags);
	ULogUtilLib::LogStringIfFlagsC(InLogFlags, TEXT("WorldType"), GetWorldTypeString(InWorldType));
	ULogUtilLib::LogNameIfFlagsC(InLogFlags, TEXT("WorldName"), InWorldName);

	if( GEngine == nullptr )
	{
		M_LOG_ERROR_IF_FLAGS(InLogFlags, TEXT("GEngine is nullptr"));
		return nullptr;
	}

	bool const bInformEngineOrWorld = (InFlags & ENewWorldFlags::SkipNotifyEngine) == ENewWorldFlags::None;
	bool const bAddToRoot = (InFlags & ENewWorldFlags::SkipAddToRoot) == ENewWorldFlags::None;

	UWorld* W = nullptr;
	{
		M_LOGBLOCK_IF_FLAGS(InLogFlags, TEXT("UWorld::CreateWorld"));
		W = UWorld::CreateWorld(InWorldType, bInformEngineOrWorld, InWorldName, /*Package*/nullptr, bAddToRoot);
		if(W == nullptr)
		{
			M_LOG_ERROR_IF_FLAGS(InLogFlags, TEXT("UWorld::CreateWorld returned nullptr"));
			return nullptr;
		}
		else
		{
			M_LOG(TEXT("World Created"));
			ULogUtilLib::LogObjectSafe(W);
		}
	}

	{
		bool const bCreateContext = (InFlags & ENewWorldFlags::SkipContextCreation) == ENewWorldFlags::None;

		if(bCreateContext)
		{
			M_LOG_IF_FLAGS(InLogFlags, TEXT("Creating FWorldContext (Due to flags)"));
			M_LOGBLOCK_IF_FLAGS(InLogFlags, TEXT("Calling UEngine::CreateNewWorldContext"));
			FWorldContext* const Context = &GEngine->CreateNewWorldContext(InWorldType);

			if(Context == nullptr)
			{
				M_LOG_ERROR_IF_FLAGS(InLogFlags, TEXT("UEngine::CreateNewWorldContext returned nullptr"));
				return nullptr;
			}
			else
			{
				M_LOG_IF_FLAGS(InLogFlags, TEXT("UEngineCreateNewWorldContext succeeded"));
			}

			Context->SetCurrentWorld(W);
		}
	}

	{
		bool const bInitializeActors = (InFlags & ENewWorldFlags::SkipInitializeActors) == ENewWorldFlags::None;
		if(bInitializeActors)
		{
			M_LOG_IF_FLAGS(InLogFlags, TEXT("Initializing actors (Due to flags)"));
			M_LOGBLOCK_IF_FLAGS(InLogFlags, TEXT("Calling UWorld::InitializeActorsForPlay"));

			FURL URL;
			W->InitializeActorsForPlay(URL, /*bResetTime*/true);
		}
	}

	{		
		bool const bBeginPlay = (InFlags & ENewWorldFlags::SkipBeginPlay) == ENewWorldFlags::None;
		if(bBeginPlay)
		{
			M_LOG_IF_FLAGS(InLogFlags, TEXT("Calling UWorld::BeginPlay (Due to flags)"));
			M_LOGBLOCK_IF_FLAGS(InLogFlags, TEXT("Calling UWorld::BeginPlay"));

			W->BeginPlay();

		}
		ULogUtilLib::LogYesNoIfFlagsC(InLogFlags, TEXT("HasBegunPlay of world is "), W->HasBegunPlay());
	}
	
	return W;
}	

FString UWorldUtilLib::GetWorldTypeString(EWorldType::Type InType)
{
	switch(InType)
	{
	case EWorldType::Type::None:
		return FString(TEXT("None"));
	case EWorldType::Type::Game:
		return FString(TEXT("Game"));
	case EWorldType::Type::Editor:
		return FString(TEXT("Editor"));
	case EWorldType::Type::PIE:
		return FString(TEXT("PIE"));
	case EWorldType::Type::EditorPreview:
		return FString(TEXT("EditorPreview"));
	case EWorldType::Type::GamePreview:
		return FString(TEXT("GamePreview"));
	case EWorldType::Type::GameRPC:
		return FString(TEXT("GameRPC"));
	case EWorldType::Type::Inactive:
		return FString(TEXT("Inactive"));
	default:
		break;
	}
	return FString(TEXT("{Unknown world type}"));
}

FString UWorldUtilLib::GetMapName_NoStreamingPrefix(UWorld* const InWorld, ELogFlags InLogFlags)
{
	check(InWorld);

	FString const MapName = InWorld->GetMapName();
	M_LOG_IF_FLAGS(InLogFlags, TEXT("UWorld::GetMapName() returned \"%s\""), *MapName);

	FString MapName_NoStreamingPrefix = MapName;
	MapName_NoStreamingPrefix.RemoveFromStart(InWorld->StreamingLevelsPrefix);

	M_LOG_IF_FLAGS(InLogFlags, TEXT("MapName(no streaming prefix): \"%s\""), *MapName_NoStreamingPrefix);
	return MapName;
}
