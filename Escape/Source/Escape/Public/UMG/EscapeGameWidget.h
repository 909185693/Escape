// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "EscapeWidget.h"
#include "EscapeGameMode_Game.h"
#include "EscapeGameWidget.generated.h"


UCLASS()
class ESCAPE_API UEscapeGameWidget : public UEscapeWidget
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "NotifyFinishMatch"))
	void ReceiveNotifyFinishMatch(EMatchStatus MatchStatus);
};
