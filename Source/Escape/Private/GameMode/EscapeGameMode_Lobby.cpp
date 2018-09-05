// Copyright 2018 by January. All Rights Reserved.

#include "EscapeGameMode_Lobby.h"
#include "EscapePlayerController.h"


AEscapeGameMode_Lobby::AEscapeGameMode_Lobby(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// 设置默认角色蓝图
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassFinder(TEXT("/Game/Blueprints/Character/EscapeCharacter_Lobby"));
	if (DefaultPawnClassFinder.Succeeded())
	{
		DefaultPawnClass = DefaultPawnClassFinder.Class;
	}

	// 设置角色控制蓝图
	PlayerControllerClass = AEscapePlayerController::StaticClass();
}
