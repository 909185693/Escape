// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Private/EscapeNetworkBase.h"
#include "EscapeServer.generated.h"


class UEscapeMessageContrl;

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
	int32 DedicatedServerPort;

	UPROPERTY(Config)
	int32 MaxBackLog;

	UPROPERTY(Config)
	int32 MaxNetworkErrorCount;

	UPROPERTY(Transient)
	UEscapeMessageContrl* EscapeMessageContrl;

	UPROPERTY(Config)
	FString EscapeMessageContrlClassName;

	UPROPERTY()
	TSubclassOf<UEscapeMessageContrl> EscapeMessageContrlClass;

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

	TArray<FEscapeSocket> ClientsSocket;

	struct FMessageData
	{
	public:
		FMessageData(void* InData, ELogicCode InCode, EErrorCode InError, FSocket* InSocket)
			: Data(InData)
			, Code(InCode)
			, Error(InError)
			, Socket(InSocket)
		{

		}

		FMessageData()
			: Data(nullptr)
			, Code(ELogicCode::INVALID)
			, Error(EErrorCode::NONE)
			, Socket(nullptr)
		{

		}

		void* Data;

		ELogicCode Code;

		EErrorCode Error;

		FSocket* Socket;
	};

	TQueue<FMessageData, EQueueMode::Mpsc> MessageQueue;

protected:
	virtual void AddMessage(void* Data, ELogicCode Code, EErrorCode Error, FSocket* ClientSocket);
};