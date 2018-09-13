// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/EscapeGameModeBase.h"
#include "EscapeGameMode_Lobby.generated.h"


class UEscapeServer;
class UEscapeClient;

/**
 * 
 */
UCLASS(Config = Game)
class ESCAPE_API AEscapeGameMode_Lobby : public AEscapeGameModeBase
{
	GENERATED_UCLASS_BODY()
	
	/**
	 * Initialize the game.
	 * The GameMode's InitGame() event is called before any other functions (including PreInitializeComponents() )
	 * and is used by the GameMode to initialize parameters and spawn its helper classes.
	 * @warning: this is called before actors' PreInitializeComponents.
	 */
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

protected:
	UPROPERTY(Transient)
	class UEscapeServer* EscapeServer;

	UPROPERTY(Config)
	TSubclassOf<UEscapeServer> EscapeServerClass;

	UPROPERTY(Config)
	FString EscapeServerClassName;

	UPROPERTY(Config)
	TSubclassOf<UEscapeClient> EscapeClientClass;
	
	
};
