// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/EscapeGameModeBase.h"
#include "EscapeGameMode_Game.generated.h"


class AEscapeCharacter;

UENUM(BlueprintType)
enum EMatchStatus
{
	Victory,
	Defeated,
	Dogfall
};

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

	/** Start play */
	virtual void StartPlay() override;

	/** Called after a successful login.  This is the first place it is safe to call replicated functions on the PlayerController. */
	virtual void PostLogin(APlayerController* NewPlayer) override;

	/** Called when a Controller with a PlayerState leaves the game or is destroyed */
	virtual void Logout(AController* Exiting) override;

	/** Called when the state transitions to WaitingToStart */
	virtual void HandleMatchIsWaitingToStart() override;

	/** Called when the state transitions to InProgress */
	virtual void HandleMatchHasStarted() override;

	/** Called when the map transitions to WaitingPostMatch */
	virtual void HandleMatchHasEnded() override;

	/** Called when the match transitions to LeavingMap */
	virtual void HandleLeavingMap() override;

	/** Called when the match transitions to Aborted */
	virtual void HandleMatchAborted() override;

	/** Overridable virtual function to dispatch the appropriate transition functions before GameState and Blueprints get SetMatchState calls. */
	virtual void OnMatchStateSet() override;

	/// AEscapeGameMode_Game
	virtual void NotifyTakeDamage(float DamageAmount, bool bKilled, AEscapeCharacter* DamageCauser, class AController* EventInstigator);

protected:
	FTimerHandle MatchStateHandle;
	FTimerHandle MatchStatusHandle;

	virtual void NotifyMatchStatus();

	virtual void RequestExit();

	/// Sync to logic server
	class UEscapeClient* GetEscapeClient() const;
	virtual void SyncNumPlayers() const;
	virtual void SyncMatchState() const;

public:
	UPROPERTY(Config)
	int32 MinPlayers;

	UPROPERTY(Config)
	int32 MaxPlayers;

protected:
	UPROPERTY(Transient)
	AController* VictoryController;

	FTimerHandle RequestExitHandle;
};
