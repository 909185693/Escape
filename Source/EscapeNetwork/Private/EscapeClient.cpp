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

bool UEscapeClient::Run()
{
	return bShouldConnected = Super::Run();
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

			if (Socket->Connect(*HostAddr))
			{
				bIsConnected = true;
			}
			else if (++ConnectCount > MaxConnectCount)
			{
				bShouldConnected = false;
			}
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
			if (Error == INVALID_DATA)
			{
				bSeriousError = true;
			}
			else if (Error == NETWORK_ERROR)
			{
				if (++ConnectCount > MaxConnectCount)
				{
					bSeriousError = true;
				}
			}
			else
			{
				ConnectCount = 0;

				FMemory::Free(Data);

				UE_LOG(LogEscapeNetwork, Log, TEXT("EscapeClient £ºRecv => Code[%d] Error[%d]"), Code, Error);
			}

			if (bSeriousError)
			{
				bIsConnected = false;

				UE_LOG(LogEscapeNetwork, Log, TEXT("EscapeClient £ºnet error => Code[%d] Error[%d]"), Code, Error);
			}
		}
	}
}