// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EscapeGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPE_API AEscapeGameModeBase : public AGameModeBase, public FNetworkNotify
{
	GENERATED_UCLASS_BODY()
	
	/** The NAME_EscapeNetDriver game connection(s) for client/server communication */
	UPROPERTY(Transient)
	class UNetDriver* NetDriver;

	// Start listening for connections.
	virtual bool Listen(FURL& InURL);

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

	//~ Begin FNetworkNotify Interface
	virtual EAcceptConnection::Type NotifyAcceptingConnection() override;
	virtual void NotifyAcceptedConnection(class UNetConnection* Connection) override;
	virtual bool NotifyAcceptingChannel(class UChannel* Channel) override;
	virtual void NotifyControlMessage(UNetConnection* Connection, uint8 MessageType, class FInBunch& Bunch) override;
	//~ End FNetworkNotify Interface
		
};
