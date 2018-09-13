// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Private/EscapeNetworkBase.h"
#include "EscapeClient.generated.h"


UCLASS(Transient, config = Engine)
class UEscapeClient : public UEscapeNetworkBase
{
	GENERATED_UCLASS_BODY()

	virtual bool Run() override;
	virtual void TickDispatch(float DeltaTime) override;
	virtual void Process() override;

protected:
	UPROPERTY(Config)
	FString ServerIP;

	UPROPERTY(Config)
	int32 ServerPort;

	int32 ConnectCount;

	UPROPERTY(Config)
	int32 MaxConnectCount;

	bool bShouldConnected;

	bool bIsConnected;
};