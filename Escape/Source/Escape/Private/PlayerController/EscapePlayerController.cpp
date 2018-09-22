// Copyright 2018 by January. All Rights Reserved.

#include "EscapePlayerController.h"
#include "EscapeGameInstance.h"
#include "GameMapsSettings.h"


AEscapePlayerController::AEscapePlayerController(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{

}

void AEscapePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() && UserWidgetClass != nullptr)
	{
		UserWidget = CreateWidget(this, UserWidgetClass);
		UserWidget->AddToViewport();
	}

	UEscapeGameInstance* GameInstance = Cast<UEscapeGameInstance>(GetGameInstance());
	EscapeClient = GameInstance ? GameInstance->GetEscapeClient() : nullptr;
	if (EscapeClient != nullptr)
	{
		EscapeClient->AddMessageCallback(ELogicCode::CLIENT_TRAVEL, this, &AEscapePlayerController::NotifyClientTravel);
	}
}

void AEscapePlayerController::Destroyed()
{
	Super::Destroyed();

	if (UserWidget != nullptr)
	{
		UserWidget->RemoveFromViewport();
		UserWidget = nullptr;
	}

	if (EscapeClient != nullptr)
	{
		EscapeClient->ClearMessageCallback(this);
	}
}

UEscapeClient* AEscapePlayerController::GetEscapeClient() const
{
	return EscapeClient;
}

void AEscapePlayerController::ReturnLobby()
{
	const UGameMapsSettings* GameMapsSettings = GetDefault<UGameMapsSettings>();
	const FString& DefaultMap = GameMapsSettings->GetGameDefaultMap();
	const FString& URL = FString::Printf(TEXT("%s"), *DefaultMap);

	ClientTravelInternal(DefaultMap, ETravelType::TRAVEL_Absolute);
}

void AEscapePlayerController::NotifyClientTravel(void* Data, EErrorCode Error)
{
	FClientTravel* Travel = (FClientTravel*)Data;

	FString URL(ANSI_TO_TCHAR(Travel->URL));

	ClientTravel(URL, TRAVEL_Absolute);
}