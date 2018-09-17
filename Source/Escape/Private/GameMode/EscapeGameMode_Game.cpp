// Copyright 2018 by January. All Rights Reserved.

#include "EscapeGameMode_Game.h"
#include "EscapePlayerState_Game.h"
#include "EscapePlayerController_Game.h"


AEscapeGameMode_Game::AEscapeGameMode_Game(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// ����Ĭ�Ͻ�ɫ��
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassFinder(TEXT("/Game/Blueprints/Character/EscapeCharacter_Player"));
	if (DefaultPawnClassFinder.Succeeded())
	{
		DefaultPawnClass = DefaultPawnClassFinder.Class;
	}

	// �������״̬��
	PlayerStateClass = AEscapePlayerState_Game::StaticClass();

	// ���ý�ɫ������
	PlayerControllerClass = AEscapePlayerController_Game::StaticClass();
}

void AEscapeGameMode_Game::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AEscapeGameMode_Game::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (NumPlayers == 0)
	{
		FPlatformMisc::RequestExit(false);
	}
}