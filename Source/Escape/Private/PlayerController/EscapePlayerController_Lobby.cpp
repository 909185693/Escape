// Copyright 2018 by January. All Rights Reserved.

#include "EscapePlayerController_Lobby.h"
#include "EscapeGameInstance.h"


AEscapePlayerController_Lobby::AEscapePlayerController_Lobby(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	// 设置默认角色类
	static ConstructorHelpers::FClassFinder<UUserWidget> UserWidgetClassFinder(TEXT("/Game/Blueprints/UMG/Lobby/Lobby"));
	if (UserWidgetClassFinder.Succeeded())
	{
		UserWidgetClass = UserWidgetClassFinder.Class;
	}
}

void AEscapePlayerController_Lobby::StartGame()
{
	JionGameRoom();
}

bool AEscapePlayerController_Lobby::JionGameRoom_Validate()
{
	return true;
}

void AEscapePlayerController_Lobby::JionGameRoom_Implementation()
{

}

void AEscapePlayerController_Lobby::BeginPlay()
{
	Super::BeginPlay();
}

