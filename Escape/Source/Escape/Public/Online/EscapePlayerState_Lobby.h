// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Online/EscapePlayerState.h"
#include "EscapePlayerState_Lobby.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPE_API AEscapePlayerState_Lobby : public AEscapePlayerState
{
	GENERATED_BODY()
	
	virtual bool IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const override;
	
	
};
