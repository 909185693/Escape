// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "EscapeNetDriver.h"
#include "OnlineSubsystemNames.h"
#include "OnlineSubsystem.h"
#include "SocketSubsystem.h"
#include "SocketsEscape.h"
#include "EscapeNetConnection.h"
#include "OnlineSubsystemEscapeTypes.h"


UEscapeNetDriver::UEscapeNetDriver(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	bIsPassthrough(false)
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
	return ISocketSubsystem::Get(bIsPassthrough ? PLATFORM_SOCKETSUBSYSTEM : ESCAPE_SUBSYSTEM);
}

bool UEscapeNetDriver::InitBase(bool bInitAsClient, FNetworkNotify* InNotify, const FURL& URL, bool bReuseAddressAndPort, FString& Error)
{
	if (bIsPassthrough)
	{
		return UIpNetDriver::InitBase(bInitAsClient, InNotify, URL, bReuseAddressAndPort, Error);
	}

	// Skip UIpNetDriver implementation
	if (!UNetDriver::InitBase(bInitAsClient, InNotify, URL, bReuseAddressAndPort, Error))
	{
		return false;
	}

	ISocketSubsystem* SocketSubsystem = GetSocketSubsystem();
	if (SocketSubsystem == NULL)
	{
		UE_LOG(LogNet, Warning, TEXT("Unable to find socket subsystem"));
		Error = TEXT("Unable to find socket subsystem");
		return false;
	}

	if(Socket == NULL)
	{
		Socket = 0;
		Error = FString::Printf( TEXT("SteamSockets: socket failed (%i)"), (int32)SocketSubsystem->GetLastErrorCode() );
		return false;
	}

	// Bind socket to our port.
	LocalAddr = SocketSubsystem->GetLocalBindAddr(*GLog);

	// Set the Steam channel (port) to communicate on (both Client/Server communicate on same "channel")
	LocalAddr->SetPort(URL.Port);

	int32 AttemptPort = LocalAddr->GetPort();
	int32 BoundPort = SocketSubsystem->BindNextPort(Socket, *LocalAddr, MaxPortCountToTry + 1, 1);
	UE_LOG(LogNet, Display, TEXT("%s bound to port %d"), *GetName(), BoundPort);
	// Success.
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
			Socket = SteamSockets->CreateSocket(FName(TEXT("EscapeClientSocket")), TEXT("Unreal client (Escape)"));
		}
		else
		{
			bIsPassthrough = true;
		}
	}

	return Super::InitConnect(InNotify, ConnectURL, Error);
}

bool UEscapeNetDriver::InitListen(FNetworkNotify* InNotify, FURL& ListenURL, bool bReuseAddressAndPort, FString& Error)
{
	return false;
}

void UEscapeNetDriver::Shutdown()
{
	Super::Shutdown();
}

bool UEscapeNetDriver::IsNetResourceValid()
{
	return false;
}
