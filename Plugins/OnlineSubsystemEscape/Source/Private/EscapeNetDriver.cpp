// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "EscapeNetDriver.h"
#include "OnlineSubsystemNames.h"
#include "OnlineSubsystem.h"
#include "SocketSubsystem.h"
#include "EscapeNetConnection.h"
#include "OnlineSubsystemEscapeTypes.h"
#include "Sockets/Public/Sockets.h"


#define ServerSocketReceiveBufferBytes 0x20000
#define ServerSocketSendBufferBytes 0x20000
#define ClientSocketReceiveBufferBytes 0x8000
#define ClientSocketSendBufferBytes 0x8000

UEscapeNetDriver::UEscapeNetDriver(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{

}

void UEscapeNetDriver::PostInitProperties()
{
	Super::PostInitProperties();
}

bool UEscapeNetDriver::IsAvailable() const
{
	// Net driver won't work if the online and socket subsystems don't exist
	IOnlineSubsystem* EscapeSubsystem = IOnlineSubsystem::Get(ESCAPE_SUBSYSTEM);
	if (EscapeSubsystem)
	{
		ISocketSubsystem* EscapeSockets = ISocketSubsystem::Get(ESCAPE_SUBSYSTEM);
		if (EscapeSockets)
		{
			return true;
		}
	}

	return false;
}

ISocketSubsystem* UEscapeNetDriver::GetSocketSubsystem()
{
	return ISocketSubsystem::Get(ESCAPE_SUBSYSTEM);
}

bool UEscapeNetDriver::InitBase(bool bInitAsClient, FNetworkNotify* InNotify, const FURL& URL, bool bReuseAddressAndPort, FString& Error)
{
	ISocketSubsystem* SocketSubsystem = GetSocketSubsystem();
	if (SocketSubsystem == NULL)
	{
		UE_LOG(LogNet, Warning, TEXT("Unable to find socket subsystem"));
		return false;
	}
	// Derived types may have already allocated a socket

	// Create the socket that we will use to communicate with
	Socket = CreateSocket();

	if (Socket == NULL)
	{
		Socket = 0;
		Error = FString::Printf(TEXT("WinSock: socket failed (%i)"), (int32)SocketSubsystem->GetLastErrorCode());
		return false;
	}

	if (Socket->SetReuseAddr(bReuseAddressAndPort) == false)
	{
		UE_LOG(LogNet, Log, TEXT("setsockopt with SO_REUSEADDR failed"));
	}

	if (Socket->SetRecvErr() == false)
	{
		UE_LOG(LogNet, Log, TEXT("setsockopt with IP_RECVERR failed"));
	}

	// Increase socket queue size, because we are polling rather than threading
	// and thus we rely on the OS socket to buffer a lot of data.
	int32 RecvSize = bInitAsClient ? ClientSocketReceiveBufferBytes : ServerSocketReceiveBufferBytes;
	int32 SendSize = bInitAsClient ? ClientSocketSendBufferBytes : ServerSocketSendBufferBytes;
	Socket->SetReceiveBufferSize(RecvSize, RecvSize);
	Socket->SetSendBufferSize(SendSize, SendSize);
	UE_LOG(LogInit, Log, TEXT("%s: Socket queue %i / %i"), SocketSubsystem->GetSocketAPIName(), RecvSize, SendSize);

	// Bind socket to our port.
	LocalAddr = SocketSubsystem->GetLocalBindAddr(*GLog);

	LocalAddr->SetPort(bInitAsClient ? GetClientPort() : URL.Port);

	if (Socket->SetNonBlocking() == false)
	{
		Error = FString::Printf(TEXT("%s: SetNonBlocking failed (%i)"), SocketSubsystem->GetSocketAPIName(),
			(int32)SocketSubsystem->GetLastErrorCode());
		return false;
	}

	return true;
}

bool UEscapeNetDriver::InitConnect(FNetworkNotify* InNotify, const FURL& ConnectURL, FString& Error)
{
	ISocketSubsystem* SteamSockets = ISocketSubsystem::Get(ESCAPE_SUBSYSTEM);
	if (SteamSockets)
	{
		// If we are opening a Steam URL, create a Steam client socket
		if (ConnectURL.Host.StartsWith(ESCAPE_URL_PREFIX))
		{
			Socket = SteamSockets->CreateSocket(ESCAPE_SOCKET_TYPE_CLIENT, TEXT("Unreal client (Escape)"));
		}
	}

	return Super::InitConnect(InNotify, ConnectURL, Error);
}

bool UEscapeNetDriver::InitListen(FNetworkNotify* InNotify, FURL& ListenURL, bool bReuseAddressAndPort, FString& Error)
{
	return Super::InitListen(InNotify, ListenURL, bReuseAddressAndPort, Error);
}

void UEscapeNetDriver::Shutdown()
{
	Super::Shutdown();
}

bool UEscapeNetDriver::IsNetResourceValid()
{
	return false;
}

FSocket* UEscapeNetDriver::CreateSocket()
{
	// Create UDP socket and enable broadcasting.
	ISocketSubsystem* SocketSubsystem = GetSocketSubsystem();

	if (SocketSubsystem == NULL)
	{
		UE_LOG(LogNet, Warning, TEXT("UIpNetDriver::CreateSocket: Unable to find socket subsystem"));
		return NULL;
	}

	return SocketSubsystem->CreateSocket(NAME_Stream, TEXT("Escape"));
}