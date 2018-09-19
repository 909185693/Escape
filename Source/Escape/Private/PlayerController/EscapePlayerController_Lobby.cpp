// Copyright 2018 by January. All Rights Reserved.

#include "EscapePlayerController_Lobby.h"
#include "EscapeGameInstance.h"


AEscapePlayerController_Lobby::AEscapePlayerController_Lobby(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	// …Ë÷√ƒ¨»œWidget¿‡
	static ConstructorHelpers::FClassFinder<UUserWidget> UserWidgetClassFinder(TEXT("/Game/Blueprints/UMG/Lobby/Lobby"));
	if (UserWidgetClassFinder.Succeeded())
	{
		UserWidgetClass = UserWidgetClassFinder.Class;
	}
}

void AEscapePlayerController_Lobby::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;

	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	SetInputMode(InputMode);
}

