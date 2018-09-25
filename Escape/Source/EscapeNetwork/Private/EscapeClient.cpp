// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "EscapeClient.h"

UEscapeClient::UEscapeClient(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
	, bIsConnected(false)
	, ServerIP("127.0.0.1")
	, ServerPort(8800)
	, ConnectCount(0)
	, MaxConnectCount(20)
{

}

bool UEscapeClient::Register(UEscapeEngine* InEngine)
{
	bShouldConnected = Super::Register(InEngine);
	
	return bShouldConnected;
}

void UEscapeClient::TickDispatch(float DeltaTime)
{
	if (Socket != nullptr)
	{
		if (!bIsConnected && bShouldConnected)
		{
			TSharedPtr<FInternetAddr> HostAddr = SocketSubsystem->CreateInternetAddr();
			bool bIsValid = false;
			HostAddr->SetIp(*ServerIP, bIsValid);
			HostAddr->SetPort(ServerPort);

			switch (Socket->GetConnectionState())
			{
			case ESocketConnectionState::SCS_NotConnected:
			case ESocketConnectionState::SCS_ConnectionError:
				Socket->Connect(*HostAddr);
				if (++ConnectCount > MaxConnectCount)
				{
					ConnectCount = 0;

					bShouldConnected = false;

					AddMessage(LC_CONNECTION, EC_NETWORKERROR);
				}
				break;
			case ESocketConnectionState::SCS_Connected:
				ConnectCount = 0;
				bIsConnected = true;
				bShouldConnected = false;
				AddMessage(LC_CONNECTION, EC_NONE);
				break;
			}
		}

		if (!MessageQueue.IsEmpty())
		{
			FMessageData MessageData;

			MessageQueue.Dequeue(MessageData);

			for (FClientMessageCallbackPtr& Callback : MessagesCallback)
			{
				if (Callback->Code == MessageData.Code)
				{
					Callback->Delegate.ExecuteIfBound(MessageData.Data, MessageData.Error);
				}
			}

			FMemory::Free(MessageData.Data);
		}
	}
}

void UEscapeClient::Process()
{
	if (bIsConnected)
	{
		check(Socket);

		void* Data = nullptr;

		ELogicCode Code;

		EErrorCode Error;

		if (RecvFrom(Socket, Data, Code, Error))
		{
			bool bSeriousError = false;
			if (Error == EC_INVALIDDATA)
			{
				bSeriousError = true;

				AddMessage(LC_CONNECTION, EC_INVALIDDATA);
			}
			else if (Error == EC_NETWORKERROR)
			{
				if (++ConnectCount > MaxConnectCount)
				{
					bSeriousError = true;

					AddMessage(LC_CONNECTION, EC_NETWORKERROR);
				}
			}
			else
			{
				ConnectCount = 0;

				AddMessage(Code, Error, Data);
			}

			if (bSeriousError)
			{
				bIsConnected = false;
			}
		}
	}
}

void UEscapeClient::Send(ELogicCode Code, int32 DataSize, void* Data)
{
	if (bIsConnected)
	{
		check(Socket);
		if (!SendTo(Socket, Code, EC_NONE, DataSize, Data))
		{
			AddMessage(LC_CONNECTION, EC_NETWORKERROR);
		}
	}
}

void UEscapeClient::Reconnect()
{
	bShouldConnected = true;

	if (Socket != nullptr)
	{
		Socket->Close();

		SocketSubsystem->DestroySocket(Socket);

		Socket = nullptr;
	}

	FString Error;
	if (!InitBase(Error))
	{
		UE_LOG(LogEscapeNetwork, Warning, TEXT("EscapeClient : %s"), *Error);
	}
}

bool UEscapeClient::IsConnected() const
{
	return bIsConnected;
}

void UEscapeClient::BeginDestroy()
{
	Super::BeginDestroy();

	MessagesCallback.Empty();
}

void UEscapeClient::AddMessage(ELogicCode Code, EErrorCode Error, void* Data)
{
	MessageQueue.Enqueue(FMessageData(Data, Code, Error));

	UE_LOG(LogEscapeNetwork, Log, TEXT("EscapeClient : Recv => Code[%d] Error[%d]"), Code, Error);
}