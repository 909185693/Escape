// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Private/EscapeNetworkBase.h"
#include "EscapeServer.generated.h"


struct FEscapeSocket
{
public:
	FEscapeSocket(FSocket* InSocket)
		: Socket(InSocket)
		, NetworkErrorCount(0)
	{

	}

	FSocket* operator->()
	{
		return Socket;
	}

	FSocket* operator*()
	{
		return Socket;
	}

	int32 NetworkErrorCount;

protected:

	FSocket* Socket;
};

UCLASS(Transient, config = Engine)
class ESCAPENETWORK_API UEscapeServer : public UEscapeNetworkBase
{
	GENERATED_UCLASS_BODY()

	virtual bool Register(UEscapeEngine* InEngine) override;
	virtual void TickDispatch(float DeltaTime) override;
	virtual void Process() override;

protected:
	UPROPERTY(Config)
	int32 ListenPort;

	UPROPERTY(Config)
	int32 MaxBackLog;

	UPROPERTY(Config)
	int32 MaxNetworkErrorCount;

	TArray<FEscapeSocket> ClientsSocket;
};