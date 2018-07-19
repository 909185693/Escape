// Fill out your copyright notice in the Description page of Project Settings.

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
