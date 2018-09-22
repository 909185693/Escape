// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "EscapeGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPE_API AEscapeGameModeBase : public AGameMode
{
	GENERATED_UCLASS_BODY()

	/**
	 * Initialize the game.
	 * The GameMode's InitGame() event is called before any other functions (including PreInitializeComponents() )
	 * and is used by the GameMode to initialize parameters and spawn its helper classes.
	 * @warning: this is called before actors' PreInitializeComponents.
	 */
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	/** Transitions to calls BeginPlay on actors. */
	virtual void StartPlay() override;
	
	/** Returns game session class to use */
	virtual TSubclassOf<AGameSession> GetGameSessionClass() const override;
	
};
