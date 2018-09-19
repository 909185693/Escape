// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EscapePlayerController.h"
#include "EscapeGameMode_Game.h"
#include "EscapePlayerController_Game.generated.h"


UCLASS()
class ESCAPE_API AEscapePlayerController_Game : public AEscapePlayerController
{
	GENERATED_UCLASS_BODY()
	
	virtual void BeginPlay() override;
	
	virtual void AddDamageInfo(float NewDamage, FVector NewHitLocation, bool bIsCrit);

	UFUNCTION(Client, Reliable)
	virtual void ClientMatchStatus(EMatchStatus MatchStatus);

};
