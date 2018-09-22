// Copyright 2018 by January. All Rights Reserved.

#include "EscapePlayerState_Lobby.h"


bool AEscapePlayerState_Lobby::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
{
	return Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);
	//return GetOwner() == RealViewer;
}
