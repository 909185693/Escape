// Copyright 2018 by January. All Rights Reserved.

#include "EscapePlayerController_Game.h"
#include "EscapeHUD_Game.h"
#include "EscapeGameWidget.h"
#include "EscapeCharacter_Player.h"


AEscapePlayerController_Game::AEscapePlayerController_Game(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	// …Ë÷√ƒ¨»œWidget¿‡
	static ConstructorHelpers::FClassFinder<UUserWidget> UserWidgetClassFinder(TEXT("/Game/Blueprints/UMG/Game/Game"));
	if (UserWidgetClassFinder.Succeeded())
	{
		UserWidgetClass = UserWidgetClassFinder.Class;
	}
}

void AEscapePlayerController_Game::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

void AEscapePlayerController_Game::AddDamageInfo(float NewDamage, FVector NewHitLocation, bool bIsCrit)
{
	AEscapeHUD_Game* HUD = Cast<AEscapeHUD_Game>(GetHUD());
	if (HUD != nullptr)
	{
		HUD->AddDamageInfo(NewDamage, NewHitLocation, bIsCrit);
	}
}

void AEscapePlayerController_Game::ClientMatchStatus_Implementation(EMatchStatus MatchStatus)
{
	SetIgnoreLookInput(true);
	SetIgnoreMoveInput(true);

	AEscapeCharacter_Player* PlayerPawn = Cast<AEscapeCharacter_Player>(GetPawn());
	if (PlayerPawn != nullptr)
	{
		PlayerPawn->SetAttackPressed(false);
	}

	bShowMouseCursor = true;

	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	SetInputMode(InputMode);

	UEscapeGameWidget* GameWidget = Cast<UEscapeGameWidget>(UserWidget);
	if (GameWidget != nullptr)
	{
		GameWidget->ReceiveNotifyFinishMatch(MatchStatus);
	}
}