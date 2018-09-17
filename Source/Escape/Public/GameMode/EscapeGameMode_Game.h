// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/EscapeGameModeBase.h"
#include "EscapeGameMode_Game.generated.h"


/**
 * 
 */
UCLASS(Config = Game)
class ESCAPE_API AEscapeGameMode_Game : public AEscapeGameModeBase
{
	GENERATED_UCLASS_BODY()
	
	/**
	 * Initialize the game.
	 * The GameMode's InitGame() event is called before any other functions (including PreInitializeComponents() )
	 * and is used by the GameMode to initialize parameters and spawn its helper classes.
	 * @warning: this is called before actors' PreInitializeComponents.
	 */
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	/** Called when a Controller with a PlayerState leaves the game or is destroyed */
	virtual void Logout(AController* Exiting) override;
};
