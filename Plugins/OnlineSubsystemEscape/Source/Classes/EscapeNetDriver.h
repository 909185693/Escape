// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "IpNetDriver.h"
#include "EscapeNetDriver.generated.h"

class Error;
class FNetworkNotify;

UCLASS(transient, config=Engine)
class UEscapeNetDriver : public UIpNetDriver
{
	GENERATED_UCLASS_BODY()

	/** Should this net driver behave as a passthrough to normal IP */
	bool bIsPassthrough;

	UPROPERTY(Config)
	int32 EscapeServerPort;

	UPROPERTY(Config)
	int32 EscapeMaxBackLog;
	
	//~ Begin UObject Interface
	virtual void PostInitProperties() override;
	//~ End UObject Interface

	//~ Begin UIpNetDriver Interface
	virtual void TickDispatch(float DeltaTime) override;
	virtual bool InitBase(bool bInitAsClient, FNetworkNotify* InNotify, const FURL& URL, bool bReuseAddressAndPort, FString& Error) override;
	virtual bool InitConnect(FNetworkNotify* InNotify, const FURL& ConnectURL, FString& Error) override;	
	virtual bool InitListen(FNetworkNotify* InNotify, FURL& ListenURL, bool bReuseAddressAndPort, FString& Error) override;
	virtual void Shutdown() override;
	//~ End UIpNetDriver Interface
};
