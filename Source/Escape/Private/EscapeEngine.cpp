// Fill out your copyright notice in the Description page of Project Settings.

#include "EscapeEngine.h"


void UEscapeEngine::Init(IEngineLoop* InEngineLoop)
{
	// Note: Lots of important things happen in Super::Init(), including spawning the player pawn in-game and
	// creating the renderer.
	Super::Init(InEngineLoop);

	for (FNetDriverDefinition& Definition : NetDriverDefinitions)
	{
		UE_LOG(LogTemp, Log, TEXT("DefName [%s] DriverClassName[%s] DriverClassNameFallback[%s]"), *Definition.DefName.ToString(), *Definition.DriverClassName.ToString(), *Definition.DriverClassNameFallback.ToString());
	}
}

