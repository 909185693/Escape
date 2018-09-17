// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlayerController/EscapePlayerController.h"
#include "EscapePlayerController_Lobby.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPE_API AEscapePlayerController_Lobby : public AEscapePlayerController
{
	GENERATED_UCLASS_BODY()

	virtual void BeginPlay() override;
};
