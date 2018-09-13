// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "EscapeNetworkBase.h"
#include "EscapeNetworkModule.h"
#include "EscapeNetworkDelegates.h"
#include "EscapeOnlineAsyncTaskManager.h"


#define SocketDescription TEXT("EscapeNetwork")

UEscapeNetworkBase::UEscapeNetworkBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
	, Socket(nullptr)
{

}

bool UEscapeNetworkBase::Run()
{
	FString Error;
	if (!InitBase(Error))
	{
		UE_LOG(LogEscapeNetwork, Warning, TEXT("EscapeServer : %s"), *Error);
		return false;
	}

	OnlineAsyncTaskThreadRunnable = new FEscapeOnlineAsyncTaskManager(this);
	check(OnlineAsyncTaskThreadRunnable);
	OnlineAsyncTaskThread = FRunnableThread::Create(OnlineAsyncTaskThreadRunnable, *FString::Printf(TEXT("EscapeOnlineAsyncTaskThread %s"), TEXT("ESCAPENETWORK")), 128 * 1024, TPri_Normal);
	check(OnlineAsyncTaskThread);
	UE_LOG_ONLINE(Verbose, TEXT("Created thread (ID:%d)."), OnlineAsyncTaskThread->GetThreadID());

	return true;
}

bool UEscapeNetworkBase::InitBase(FString& Error)
{
	SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	if (SocketSubsystem == nullptr)
	{
		UE_LOG(LogEscapeNetwork, Warning, TEXT("Unable to find socket subsystem"));
		return false;
	}

	Socket = SocketSubsystem->CreateSocket(NAME_Stream, SocketDescription);
	if (Socket == nullptr)
	{
		Socket = 0;
		Error = FString::Printf(TEXT("Socket: create failed (%i)"), (int32)SocketSubsystem->GetLastErrorCode());
		return false;
	}

	if (!Socket->SetNonBlocking())
	{
		Error = FString::Printf(TEXT("Socket: set non blocking (%i)"), (int32)SocketSubsystem->GetLastErrorCode());
		return false;
	}

	return true;
}

bool UEscapeNetworkBase::SendTo(FSocket* SendSocket, ELogicCode Code, int32 DataSize, void* Data)
{
	int32 Count = DataSize + sizeof(FDataHeader);
	void* SendData = FMemory::Malloc(Count);

	FDataHeader* DataHeader = (FDataHeader*)SendData;
	DataHeader->Code = Code;
	DataHeader->Size = DataSize;

	FMemory::Memcpy((void*)(DataHeader + 1), Data, DataSize);

	int32 BytesSent = 0;
	bool bResult = SendSocket->Send((uint8*)SendData, Count, BytesSent);

	FMemory::Free(SendData);

	return bResult;
}

static const FTimespan WaitTime(2000);
bool UEscapeNetworkBase::RecvFrom(FSocket* RecvSocket, void* OutData, ELogicCode& OutCode, EErrorCode& OutError)
{
	if (!RecvSocket->Wait(ESocketWaitConditions::WaitForRead, WaitTime))
	{
		return false;
	}

	int32 BytesRead = 0;

	const int32 HeaderSize = sizeof(FDataHeader);
	uint8 HeaderData[HeaderSize];
	RecvSocket->Recv(HeaderData, HeaderSize, BytesRead);

	FDataHeader* DataHander = (FDataHeader*)HeaderData;

	if (HeaderSize != BytesRead || !DataHander->IsValid())
	{
		OutError = EErrorCode::INVALID_DATA;

		return true;
	}

	void* RecvData = FMemory::Malloc(DataHander->Size);
	
	while (RecvSocket != nullptr)
	{
		bool bOk = false;
		{
			bOk = RecvSocket->Recv((uint8*)RecvData, DataHander->Size, BytesRead);
		}

		if (bOk)
		{
			if (BytesRead == 0)
			{
				OutData = RecvData;
				OutCode = DataHander->Code;
				OutError = DataHander->Error;
			}
		}

		OutError = EErrorCode::NETWORK_ERROR;

		return false;
	}

	return true;
}

void UEscapeNetworkBase::SetNetworkNotify(FEscapeNetworkNotify* NetworkNotify)
{

}

//void UEscapeNetworkBase::AddMessageCallback(UObject* Object, )
//{
//	FMessageCallback MessageCallback;
//
//	MessageCallback.MessageDelegate.BindUObject(this, )
//}

void UEscapeNetworkBase::BeginDestroy()
{
	Super::BeginDestroy();

	if (OnlineAsyncTaskThread)
	{
		// Destroy the online async task thread
		delete OnlineAsyncTaskThread;
		OnlineAsyncTaskThread = nullptr;
	}

	if (OnlineAsyncTaskThreadRunnable)
	{
		delete OnlineAsyncTaskThreadRunnable;
		OnlineAsyncTaskThreadRunnable = nullptr;
	}

	if (SocketSubsystem)
	{
		SocketSubsystem->DestroySocket(Socket);
		Socket = nullptr;
	}

	MessageCallbacks.Empty();
}