// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EscapePlayerController.h"
#include "EscapeNetwork.h"
#include "EscapePlayerController_Game.generated.h"


UCLASS()
class ESCAPE_API AEscapePlayerController_Game : public APlayerController
{
	GENERATED_UCLASS_BODY()
	
	virtual void BeginPlay() override;
	
};
