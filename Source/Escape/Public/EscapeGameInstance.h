// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "Online.h"
#include "Engine/GameInstance.h"
#include "EscapeGameInstance.generated.h"


class UEscapeClient;

UCLASS()
class ESCAPE_API UEscapeGameInstance : public UGameInstance
{
	GENERATED_UCLASS_BODY()
	
	class AEscapeGameSession* GetGameSession() const;

	virtual void Init() override;
	virtual void OnStart() override;
	
	/// UEscapeGameInstance
	class UEscapeClient* GetEscapeClient() const;

protected:
	UPROPERTY(Transient)
	UEscapeClient* EscapeClient;

	UPROPERTY(Config)
	FString EscapeClientClassName;

	UPROPERTY(Config)
	TSubclassOf<UEscapeClient> EscapeClientClass;
};
