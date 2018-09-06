// Copyright 2018 by January. All Rights Reserved.

#include "EscapeGameMode_Lobby.h"
#include "EscapePlayerState_Lobby.h"
#include "EscapePlayerController_Lobby.h"

AEscapeGameMode_Lobby::AEscapeGameMode_Lobby(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// ����Ĭ�Ͻ�ɫ��
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassFinder(TEXT("/Game/Blueprints/Character/EscapeCharacter_Lobby"));
	if (DefaultPawnClassFinder.Succeeded())
	{
		DefaultPawnClass = DefaultPawnClassFinder.Class;
	}

	// �������״̬��
	PlayerStateClass = AEscapePlayerState_Lobby::StaticClass();

	// ���ý�ɫ������
	PlayerControllerClass = AEscapePlayerController_Lobby::StaticClass();
}
