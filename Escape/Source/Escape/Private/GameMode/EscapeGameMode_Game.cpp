// Copyright 2018 by January. All Rights Reserved.

#include "EscapeGameMode_Game.h"
#include "EscapeGameSession.h"
#include "EscapeNetwork.h"
#include "EscapeCharacter.h"
#include "EscapeHUD_Game.h"
#include "EscapeGameState.h"
#include "EscapePlayerState_Game.h"
#include "EscapePlayerController_Game.h"


AEscapeGameMode_Game::AEscapeGameMode_Game(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, MinPlayers(1)
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

	// 设置HUD类
	HUDClass = AEscapeHUD_Game::StaticClass();

	// 设置游戏状态类
	GameStateClass = AEscapeGameState::StaticClass();
}

void AEscapeGameMode_Game::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AEscapeGameMode_Game::StartPlay()
{
	// Don't call super, this class handles begin play/match start itself
	if (MatchState == MatchState::EnteringMap)
	{
		SetMatchState(MatchState::WaitingToStart);
	}

	bDelayedStart = true;
}

void AEscapeGameMode_Game::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	SyncNumPlayers();
}

void AEscapeGameMode_Game::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	SyncNumPlayers();

	if (NumPlayers == 0)
	{
		AbortMatch();
	}
}

void AEscapeGameMode_Game::HandleMatchIsWaitingToStart()
{
	Super::HandleMatchIsWaitingToStart();
}
	 
void AEscapeGameMode_Game::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
}

void AEscapeGameMode_Game::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();

	GetWorld()->GetTimerManager().SetTimer(MatchStatusHandle, this, &AEscapeGameMode_Game::NotifyMatchStatus, 5.f, false);
}

void AEscapeGameMode_Game::HandleLeavingMap()
{
	Super::HandleLeavingMap();
	
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		AEscapePlayerController_Game* PlayerController = Cast<AEscapePlayerController_Game>(It->Get());
		if (PlayerController != nullptr)
		{
			PlayerController->ReturnLobby();
		}
	}

	AbortMatch();
}
	 
void AEscapeGameMode_Game::HandleMatchAborted()
{
	Super::HandleMatchAborted();

	RequestExit();
}

void AEscapeGameMode_Game::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	SyncMatchState();
}

void AEscapeGameMode_Game::NotifyTakeDamage(float DamageAmount, bool bKilled, AEscapeCharacter* DamageCauser, class AController* EventInstigator)
{
	if (bKilled)
	{
		if (MatchState == MatchState::InProgress)
		{
			bool bMatchFinished = true;
			for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
			{
				APlayerController* PlayerController = It->Get();
				if (PlayerController != EventInstigator)
				{
					AEscapeCharacter* Player = Cast<AEscapeCharacter>(PlayerController->GetPawn());
					if (Player && Player->IsAlive())
					{
						bMatchFinished = false;
					}
				}
			}

			if (bMatchFinished)
			{
				VictoryController = EventInstigator;

				EndMatch();
			}
		}
		else
		{

		}
	}
}

void AEscapeGameMode_Game::NotifyMatchStatus()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		AEscapePlayerController_Game* PlayerController = Cast<AEscapePlayerController_Game>(It->Get());
		if (PlayerController == VictoryController)
		{
			PlayerController->ClientMatchStatus(EMatchStatus::Victory);
		}
		else
		{
			PlayerController->ClientMatchStatus(EMatchStatus::Defeated);
		}
	}

	GetWorld()->GetTimerManager().SetTimer(MatchStateHandle, this, &AGameMode::StartToLeaveMap, 10.f, false);
}

class UEscapeClient* AEscapeGameMode_Game::GetEscapeClient() const
{
	AEscapeGameSession* EscapeGameSession = Cast<AEscapeGameSession>(GameSession);
	return EscapeGameSession ? Cast<UEscapeClient>(EscapeGameSession->GetEscapeClient()) : nullptr;
}

void AEscapeGameMode_Game::SyncNumPlayers() const
{
	UEscapeClient* EscapeClient = GetEscapeClient();
	if (EscapeClient != nullptr)
	{
		int32 NewNumPlayers = NumPlayers;

		EscapeClient->Send(ELogicCode::NUMPLAYERS, sizeof(NumPlayers), &NewNumPlayers);
	}
}

void AEscapeGameMode_Game::SyncMatchState() const
{
	UEscapeClient* EscapeClient = GetEscapeClient();
	if (EscapeClient != nullptr)
	{
		EMatchState NewMatchState;
		if (MatchState == MatchState::EnteringMap)
		{
			NewMatchState = EMatchState::EnteringMap;
		}
		else if (MatchState == MatchState::WaitingToStart)
		{
			NewMatchState = EMatchState::WaitingToStart;
		}
		else if (MatchState == MatchState::InProgress)
		{
			NewMatchState = EMatchState::InProgress;
		}
		else if (MatchState == MatchState::WaitingPostMatch)
		{
			NewMatchState = EMatchState::WaitingPostMatch;
		}
		else if (MatchState == MatchState::LeavingMap)
		{
			NewMatchState = EMatchState::LeavingMap;
		}
		else if (MatchState == MatchState::Aborted)
		{
			NewMatchState = EMatchState::Aborted;
		}

		EscapeClient->Send(ELogicCode::MATCH_STATE, sizeof(NewMatchState), &NewMatchState);
	}
}

void AEscapeGameMode_Game::RequestExit()
{
#if UE_BUILD_DEVELOPMENT
	if (!GetWorld()->IsPlayInEditor())
#endif
	{
		FPlatformMisc::RequestExit(false);
	}
}