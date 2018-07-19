// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EscapeGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPE_API AEscapeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	/** Returns game session class to use */
	virtual TSubclassOf<AGameSession> GetGameSessionClass() const override;
	
	
};
