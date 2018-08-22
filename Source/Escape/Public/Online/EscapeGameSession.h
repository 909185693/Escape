// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "Online.h"
#include "GameFramework/GameSession.h"
#include "EscapeGameSession.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPE_API AEscapeGameSession : public AGameSession
{
	GENERATED_BODY()
	
	/**
	* Called when this instance is starting up as a dedicated server
	*/
	virtual void RegisterServer() override;
		
};
