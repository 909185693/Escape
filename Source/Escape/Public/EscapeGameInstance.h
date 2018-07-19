// Fill out your copyright notice in the Description page of Project Settings.

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
