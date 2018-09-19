// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "Online.h"
#include "Engine/GameInstance.h"
#include "EscapeSystemTypes.h"
#include "EscapeGameInstance.generated.h"


class UEscapeClient;
class UEscapeSystem;

UCLASS()
class ESCAPE_API UEscapeGameInstance : public UGameInstance
{
	GENERATED_UCLASS_BODY()
	
	class AEscapeGameSession* GetGameSession() const;

	virtual void Init() override;
	virtual void OnStart() override;
	
	/// UEscapeGameInstance
	class UEscapeClient* GetEscapeClient() const;
	class UEscapeSystem* GetEscapeSystem() const;

protected:
	UPROPERTY(Transient)
	UEscapeClient* EscapeClient;

	UPROPERTY(Transient)
	UEscapeSystem* EscapeSystem;

	UPROPERTY(Config)
	FString EscapeClientClassName;

	UPROPERTY(Config)
	TSubclassOf<UEscapeClient> EscapeClientClass;
};
