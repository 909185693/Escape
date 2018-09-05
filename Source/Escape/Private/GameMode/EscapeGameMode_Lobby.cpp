// Copyright 2018 by January. All Rights Reserved.

#include "EscapeGameMode_Lobby.h"
#include "EscapePlayerController.h"


AEscapeGameMode_Lobby::AEscapeGameMode_Lobby(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// ����Ĭ�Ͻ�ɫ��ͼ
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassFinder(TEXT("/Game/Blueprints/Character/EscapeCharacter_Lobby"));
	if (DefaultPawnClassFinder.Succeeded())
	{
		DefaultPawnClass = DefaultPawnClassFinder.Class;
	}

	// ���ý�ɫ������ͼ
	PlayerControllerClass = AEscapePlayerController::StaticClass();
}
