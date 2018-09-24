// Copyright 2018 by January. All Rights Reserved.

#include "EscapeGameState.h"
#include "UnrealNetwork.h"
#include "GameFramework/GameMode.h"


AEscapeGameState::AEscapeGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MatchStartCountDown = 60.f;
}

void AEscapeGameState::HandleMatchIsWaitingToStart()
{
	Super::HandleMatchIsWaitingToStart();

	if (Role == ROLE_Authority)
	{
		GetWorld()->GetTimerManager().SetTimer(MatchStartHandle, this, &AEscapeGameState::HandleMatchStartCountDown, 1.f, true);
	}
}

void AEscapeGameState::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
}

void AEscapeGameState::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
}

void AEscapeGameState::HandleLeavingMap()
{
	Super::HandleLeavingMap();
}

void AEscapeGameState::HandleMatchStartCountDown()
{
	if (MatchState == MatchState::WaitingToStart)
	{
		if (--MatchStartCountDown <= 0)
		{
			AGameMode* GameMode = GetWorld()->GetAuthGameMode<AGameMode>();
			if (GameMode != nullptr)
			{
				GameMode->StartMatch();
			}

			GetWorld()->GetTimerManager().ClearTimer(MatchStartHandle);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(MatchStartHandle);
	}
}

void AEscapeGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEscapeGameState, MatchStartCountDown);
}
