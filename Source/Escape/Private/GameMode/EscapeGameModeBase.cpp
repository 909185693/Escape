// Copyright 2018 by January. All Rights Reserved.

#include "EscapeGameModeBase.h"
#include "EscapeGameSession.h"
#include "EscapePlayerController.h"


AEscapeGameModeBase::AEscapeGameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// 设置默认角色蓝图
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassFinder(TEXT("/Game/Blueprints/Character/EscapeCharacter_Player"));
	if (DefaultPawnClassFinder.Succeeded())
	{
		DefaultPawnClass = DefaultPawnClassFinder.Class;
	}

	// 设置角色控制蓝图
	PlayerControllerClass = AEscapePlayerController::StaticClass();
}

void AEscapeGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AEscapeGameModeBase::StartPlay()
{
	Super::StartPlay();
}

/** Returns game session class to use */
TSubclassOf<AGameSession> AEscapeGameModeBase::GetGameSessionClass() const
{
	return AEscapeGameSession::StaticClass();
}
