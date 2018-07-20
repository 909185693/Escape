// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameEngine.h"
#include "EscapeEngine.generated.h"

/**
 * 
 */
UCLASS(config=Engine, transient)
class ESCAPE_API UEscapeEngine : public UGameEngine
{
	GENERATED_BODY()
	
	/* Hook up specific callbacks */
	virtual void Init(IEngineLoop* InEngineLoop) override;
	
	
};
