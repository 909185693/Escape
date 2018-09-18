// Copyright 2018 by January. All Rights Reserved.

#include "EscapeGameMode_Game.h"
#include "EscapePlayerState_Game.h"
#include "EscapePlayerController_Game.h"
#include "EscapeGameSession.h"


AEscapeGameMode_Game::AEscapeGameMode_Game(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, MinPlayers(2)
	, MaxPlayers(5)
{
	// 设置默认角色类
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassFinder(TEXT("/Game/Blueprints/Character/EscapeCharacter_Kwang"));
	if (DefaultPawnClassFinder.Succeeded())
	{
		DefaultPawnClass = DefaultPawnClassFinder.Class;
	}

	// 设置玩家状态类
	PlayerStateClass = AEscapePlayerState_Game::StaticClass();

	// 设置角色控制类
	PlayerControllerClass = AEscapePlayerController_Game::StaticClass();
}

void AEscapeGameMode_Game::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AEscapeGameMode_Game::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	SyncGameInfo();
}

void AEscapeGameMode_Game::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	SyncGameInfo();

	if (NumPlayers == 0)
	{
		FPlatformMisc::RequestExit(false);
	}
}

void AEscapeGameMode_Game::SyncGameInfo()
{
	AEscapeGameSession* EscapeGameSession = Cast<AEscapeGameSession>(GameSession);
	UEscapeClient* EscapeClient = EscapeGameSession ? Cast<UEscapeClient>(EscapeGameSession->GetEscapeClient()) : nullptr;
	if (EscapeClient != nullptr)
	{
		FGameInfo GameInfo;
		GameInfo.NumPlayers = NumPlayers;

		EscapeClient->Send(ELogicCode::GAME_INFO, sizeof(FGameInfo), &GameInfo);
	}
}