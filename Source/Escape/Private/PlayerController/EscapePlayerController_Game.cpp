// Copyright 2018 by January. All Rights Reserved.

#include "EscapePlayerController_Game.h"
#include "EscapeGameInstance.h"


AEscapePlayerController_Game::AEscapePlayerController_Game(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{

}

void AEscapePlayerController_Game::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}