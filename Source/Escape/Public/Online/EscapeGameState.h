// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "EscapeGameState.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPE_API AEscapeGameState : public AGameState
{
	GENERATED_UCLASS_BODY()
	
	/** Called when the state transitions to WaitingToStart */
	virtual void HandleMatchIsWaitingToStart() override;

	/** Called when the state transitions to InProgress */
	virtual void HandleMatchHasStarted() override;

	/** Called when the map transitions to WaitingPostMatch */
	virtual void HandleMatchHasEnded() override;

	/** Called when the match transitions to LeavingMap */
	virtual void HandleLeavingMap() override;

protected:
	/// AEscapeGameState
	virtual void HandleMatchStartCountDown();

protected:
	FTimerHandle MatchStartHandle;

	UPROPERTY(BlueprintReadOnly, Transient, Replicated)
	int32 MatchStartCountDown;
};
