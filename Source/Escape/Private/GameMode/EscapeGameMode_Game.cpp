// Copyright 2018 by January. All Rights Reserved.

#include "EscapeGameMode_Game.h"
#include "EscapeGameSession.h"
#include "EscapeCharacter.h"
#include "EscapeHUD_Game.h"
#include "EscapePlayerState_Game.h"
#include "EscapePlayerController_Game.h"


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

	// 设置HUD类
	HUDClass = AEscapeHUD_Game::StaticClass();
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
		RequestExit();
	}
}

void AEscapeGameMode_Game::FinishMatch()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		AEscapePlayerController_Game* PlayerController = Cast<AEscapePlayerController_Game>(It->Get());
		if (PlayerController == EventInstigator)
		{
			PlayerController->ClientMatchStatus(EMatchStatus::Victory);
		}
		else
		{
			PlayerController->ClientMatchStatus(EMatchStatus::Defeated);
		}
	}

	FTimerHandle RequestExit;

	GetWorld()->GetTimerManager().SetTimer(RequestExit, this, &AEscapeGameMode_Game::RequestExit, 60.f, false);
}

void AEscapeGameMode_Game::NotifyPlayerTakeDamage(float DamageAmount, bool bKilled, AEscapeCharacter* DamageCauser, class AController* EventInstigator)
{
	if (bKilled)
	{
		if (!bFinishMatch)
		{
			bFinishMatch = true;

			for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
			{
				APlayerController* PlayerController = It->Get();
				if (PlayerController != EventInstigator)
				{
					AEscapeCharacter* Player = Cast<AEscapeCharacter>(PlayerController->GetPawn());
					if (Player && Player->IsAlive())
					{
						bFinishMatch = false;
					}
				}
			}

			if (bFinishMatch)
			{
				VictoryController = EventInstigator;

				FTimerHandle FinishMatchHandle;

				GetWorld()->GetTimerManager().SetTimer(FinishMatchHandle, this, &AEscapeGameMode_Game::FinishMatch, 5.f, false);
			}
		}
		else
		{

		}
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

void AEscapeGameMode_Game::RequestExit()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		AEscapePlayerController_Game* PlayerController = Cast<AEscapePlayerController_Game>(It->Get());
		if (PlayerController != nullptr)
		{
			PlayerController->ReturnLobby();
		}
	}

#if UE_BUILD_DEVELOPMENT
	if (!GetWorld()->IsPlayInEditor())
#endif
	{
		FPlatformMisc::RequestExit(false);
	}
}