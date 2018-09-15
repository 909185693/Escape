// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Private/EscapeNetworkBase.h"
#include "EscapeClient.generated.h"


UCLASS(Transient, config = Engine)
class ESCAPENETWORK_API UEscapeClient : public UEscapeNetworkBase
{
	GENERATED_UCLASS_BODY()

	virtual bool Register(UEscapeEngine* InEngine) override;
	virtual void Process() override;

	/// EscapeClient
public:
	virtual void TickDispatch(float DeltaTime) override;
	virtual void Send(ELogicCode Code, int32 DataSize, void* Data);
	virtual void Reconnect();

protected:
	virtual void AddMessage(void* Data, ELogicCode Code, EErrorCode Error);

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

	struct FMessageData
	{
	public:
		FMessageData(void* InData, ELogicCode InCode, EErrorCode InError)
			: Data(InData)
			, Code(InCode)
			, Error(InError)
		{

		}

		FMessageData()
			: Data(nullptr)
			, Code(ELogicCode::INVALID)
			, Error(EErrorCode::NONE)
		{

		}

		void* Data;

		ELogicCode Code;

		EErrorCode Error;
	};

	TQueue<FMessageData, EQueueMode::Mpsc> MessageQueue;
};