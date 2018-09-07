// Copyright 2018 by January. All Rights Reserved.

#include "EscapeEngine.h"
#include "EscapeGameInstance.h"
#include "GameMapsSettings.h"


void UEscapeEngine::Init(IEngineLoop* InEngineLoop)
{
	// Note: Lots of important things happen in Super::Init(), including spawning the player pawn in-game and
	// creating the renderer.
	Super::Init(InEngineLoop);

	for (FNetDriverDefinition& Definition : NetDriverDefinitions)
	{
		UE_LOG(LogTemp, Log, TEXT("DefName [%s] DriverClassName[%s] DriverClassNameFallback[%s]"), *Definition.DefName.ToString(), *Definition.DriverClassName.ToString(), *Definition.DriverClassNameFallback.ToString());
	}

	UEscapeGameInstance* NewGameInstance = NewObject<UEscapeGameInstance>(this, UEscapeGameInstance::StaticClass());
	check(NewGameInstance);

	EscapeGameInstances.Add(NewGameInstance);

	NewGameInstance->InitializeGameInstance(8899);
}

