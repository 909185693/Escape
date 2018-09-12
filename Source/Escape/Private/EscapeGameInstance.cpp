// Copyright 2018 by January. All Rights Reserved.

#include "EscapeGameInstance.h"
#include "CrasheyeHelper.h"
#include "EscapeEngine.h"


UEscapeGameInstance::UEscapeGameInstance(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)

{

}

class AEscapeGameSession* UEscapeGameInstance::GetGameSession() const
{
	return nullptr;
}

void UEscapeGameInstance::OnStart()
{
	Super::OnStart();

	// game requires the ability to ID users.
	const auto OnlineSub = IOnlineSubsystem::Get();
	check(OnlineSub);
	const auto IdentityInterface = OnlineSub->GetIdentityInterface();
	check(IdentityInterface.IsValid());

	UCrasheyeHelper::CrasheyeSetUserIdentifier(TEXT("TestCrasheye"));
	UCrasheyeHelper::CrasheyeSetAppVersion(TEXT("1.0.0"));

	UE_LOG(LogTemp, Log, TEXT("OnlineSub [%s] IdentityInterface [%s]"), *OnlineSub->GetSubsystemName().ToString(), *IdentityInterface->GetPlayerNickname(0));
}

<<<<<<< HEAD
=======
void UEscapeGameInstance::InitializeGameInstance(int32 Port)
{
	FURL URL;
	URL.Protocol = TEXT("unreal");
	URL.Port = Port;
	URL.Op.Add(TEXT("Name=Player"));
	URL.Op.Add(TEXT("Game=Game"));
	URL.Map = TEXT("/Game/Maps/EscapeMap");

	FString Error = TEXT("");

	// Creates the world context. This should be the only WorldContext that ever gets created for this GameInstance.
	WorldContext = &GetEngine()->CreateNewWorldContext(EWorldType::Game);
	check(WorldContext);
	WorldContext->OwningGameInstance = this;
	WorldContext->LastURL = URL;
	WorldContext->LastRemoteURL = URL;

	// In standalone create a dummy world from the beginning to avoid issues of not having a world until LoadMap gets us our real world
	UPackage* WorldPackage = LoadPackage(nullptr, *URL.Map, LOAD_None);
	check(WorldPackage);

	//NETWORK_PROFILER(GNetworkProfiler.TrackSessionChange(true, URL));
	MALLOC_PROFILER(FMallocProfiler::SnapshotMemoryLoadMapStart(URL.Map));

	PreloadContentForURL(URL);

	const FName MapName = FName(*FString::Printf(TEXT("EscapeMap_%d"), Port));

	UWorld* NewWorld = UWorld::CreateWorld(EWorldType::Game, false, MapName, WorldPackage, true);
	check(NewWorld);
	NewWorld->SetGameInstance(this);

	FDisableHitchDetectorScope SuspendHitchDetector;

	WorldContext->SetCurrentWorld(NewWorld);

#if !UE_BUILD_SHIPPING
	NewWorld->bCreateRenderStateForHiddenComponents = true;
#endif

	// send a callback message
	FCoreUObjectDelegates::PreLoadMap.Broadcast(URL.Map);
	
	NewWorld->PersistentLevel->HandleLegacyMapBuildData();

	if (!NewWorld->bIsWorldInitialized)
	{
		NewWorld->InitWorld();
	}

	if (!NewWorld->SetGameMode(URL))
	{
		UE_LOG(LogTemp, Warning, TEXT("New world create game mode failure! map[%s] listen port %d."), *URL.Map, Port);
	}

	if (!NewWorld->Listen(URL))
	{
		UE_LOG(LogTemp, Warning, TEXT("New world begin listen failure! map[%s] listen port %d."), *URL.Map, Port);
	}
	
	// Process global shader results before we try to render anything
	// Do this before we register components, as USkinnedMeshComponents require the GPU skin cache global shaders when creating render state.
	if (GShaderCompilingManager)
	{
		GShaderCompilingManager->ProcessAsyncResults(false, true);
	}

	// load any per-map packages
	check(NewWorld->PersistentLevel);
	GetEngine()->LoadPackagesFully(NewWorld, FULLYLOAD_Map, NewWorld->PersistentLevel->GetOutermost()->GetName());

	// Make sure "always loaded" sub-levels are fully loaded
	NewWorld->FlushLevelStreaming(EFlushLevelStreamingType::Visibility);

	// Note that AI system will be created only if ai-system-creation conditions are met
	NewWorld->CreateAISystem();

	// Initialize gameplay for the level.
	NewWorld->InitializeActorsForPlay(URL);

	// calling it after InitializeActorsForPlay has been called to have all potential bounding boxed initialized
	FNavigationSystem::AddNavigationSystemToWorld(*NewWorld, FNavigationSystemRunMode::GameMode);

	// Spawn play actors for all active local players
	if (WorldContext->OwningGameInstance != NULL)
	{
		for (auto It = WorldContext->OwningGameInstance->GetLocalPlayerIterator(); It; ++It)
		{
			FString Error2;
			if (!(*It)->SpawnPlayActor(URL.ToString(1), Error2, NewWorld))
			{
				UE_LOG(LogTemp, Fatal, TEXT("Couldn't spawn player: %s"), *Error2);
			}
		}
	}

	NewWorld->BeginPlay();

	// send a callback message
	FCoreUObjectDelegates::PostLoadMapWithWorld.Broadcast(NewWorld);

	NewWorld->bWorldWasLoadedThisTick = true;

	UE_LOG(LogTemp, Warning, TEXT("Create new world[%s] success! begin listen port[%d]"), *GetNameSafe(NewWorld), Port);

	Init();

	//UEngine* const Engine = GetEngine();
	//check(Engine);
	//if (Engine->LoadMap(*WorldContext, URL, nullptr, Error))
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Load new map success! map[%s] listen port %d."), *URL.Map, Port);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Load new map failure. map[%s] error[%s]"), *URL.Map, *Error);
	//}
}


>>>>>>> parent of f085c69... Socket
