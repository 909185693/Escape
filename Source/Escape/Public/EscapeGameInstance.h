// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "Online.h"
#include "Engine/GameInstance.h"
#include "EscapeGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPE_API UEscapeGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	class AEscapeGameSession* GetGameSession() const;

	virtual void Init() override;
		
};
