// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "EscapeServer.h"
#include "EscapeMessageContrl.h"


UEscapeServer::UEscapeServer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, ListenPort(8800)
	, DedicatedServerPort(9800)
	, MaxBackLog(1024)
	, MaxNetworkErrorCount(10)
	, EscapeMessageContrlClassName(TEXT("/Script/EscapeNetwork.EscapeMessageContrl"))
{

}

bool UEscapeServer::Register(UEscapeEngine* InEngine)
{
	if (Super::Register(InEngine))
	{
		TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		InternetAddr->SetPort(ListenPort);

		check(Socket);
		if (!Socket->Bind(InternetAddr.Get()))
		{
			UE_LOG(LogEscapeNetwork, Warning, TEXT("EscapeServer : Failed to bind on port[%d]"), ListenPort);
			return false;
		}

		if (!Socket->Listen(MaxBackLog))
		{
			UE_LOG(LogEscapeNetwork, Warning, TEXT("EscapeServer : Failed to listen on port[%d]"), ListenPort);
			return false;
		}

		EscapeMessageContrlClass = LoadClass<UEscapeMessageContrl>(NULL, *EscapeMessageContrlClassName, NULL, LOAD_None, NULL);
		if (EscapeMessageContrlClass != nullptr)
		{
			if (EscapeMessageContrl == nullptr)
			{
				EscapeMessageContrl = NewObject<UEscapeMessageContrl>(GetTransientPackage(), EscapeMessageContrlClass);
			}

			if (EscapeMessageContrl != nullptr)
			{
				EscapeMessageContrl->Register(this);
			}
		}
		else
		{
			UE_LOG(LogEscapeNetwork, Error, TEXT("Failed to load class '%s'"), *EscapeMessageContrlClassName);
		}

		UE_LOG(LogEscapeNetwork, Log, TEXT("EscapeServer : listening on port %i"), ListenPort);
	}

	return true;
}

void UEscapeServer::TickDispatch(float DeltaTime)
{

}

void UEscapeServer::Process()
{
	check(SocketSubsystem);

	check(Socket);

	FSocket* AcceptSocket = Socket->Accept(TEXT("ESCAPENETWORK"));
	if (AcceptSocket != nullptr)
	{
		ClientsSocket.Add(FEscapeSocket(AcceptSocket));
	}

	// 消息接收
	TArray<FEscapeSocket>::TIterator It(ClientsSocket);
	for (; It; ++It)
	{
		FEscapeSocket& ClientSocket = *It;

		void* Data = nullptr;

		ELogicCode Code;

		EErrorCode Error;

		if (RecvFrom(*ClientSocket, Data, Code, Error))
		{
			bool bSeriousError = false;
			if (Error == INVALID_DATA)
			{
				bSeriousError = true;
			}
			else if (Error == NETWORK_ERROR)
			{
				if (++ClientSocket.NetworkErrorCount > MaxNetworkErrorCount)
				{
					bSeriousError = true;
				}
			}
			else
			{
				ClientSocket.NetworkErrorCount = 0;

				ExecuteMessageCallback(*ClientSocket, Code, Error, Data);

				FMemory::Free(Data);

				UE_LOG(LogEscapeNetwork, Log, TEXT("EscapeServer : Recv => Code[%d] Error[%d]"), Code, Error);
			}

			if (bSeriousError)
			{
				ClientSocket->Close();

				SocketSubsystem->DestroySocket(*ClientSocket);

				It.RemoveCurrent();

				UE_LOG(LogEscapeNetwork, Log, TEXT("EscapeServer : net error => Code[%d] Error[%d]"), Code, Error);
			}
		}
	}
}