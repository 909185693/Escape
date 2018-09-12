// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "EscapeNetConnection.h"
#include "OnlineSubsystemNames.h"
#include "OnlineSubsystem.h"
#include "SocketSubsystem.h"
#include "EscapeNetDriver.h"
#include "OnlineSubsystemEscapeTypes.h"


UEscapeNetConnection::UEscapeNetConnection(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	bIsPassthrough(false)
{
}

void UEscapeNetConnection::InitLocalConnection(UNetDriver* InDriver, class FSocket* InSocket, const FURL& InURL, EConnectionState InState, int32 InMaxPacket, int32 InPacketOverhead)
{
	bIsPassthrough = InURL.Host.StartsWith(ESCAPE_URL_PREFIX) ? false : true;
	
	Super::InitLocalConnection(InDriver, InSocket, InURL, InState, InMaxPacket, InPacketOverhead);
	if (!bIsPassthrough && RemoteAddr.IsValid())
	{
		//FSocketSubsystemEscape* SocketSubsystem = (FSocketSubsystemEscape*)ISocketSubsystem::Get(ESCAPE_SUBSYSTEM);
		//if (SocketSubsystem)
		//{
		//	//SocketSubsystem->RegisterConnection(this);
		//}
	}
}

void UEscapeNetConnection::InitRemoteConnection(UNetDriver* InDriver, class FSocket* InSocket, const FURL& InURL, const class FInternetAddr& InRemoteAddr, EConnectionState InState, int32 InMaxPacket, int32 InPacketOverhead)
{
	bIsPassthrough = ((UEscapeNetDriver*)InDriver)->bIsPassthrough;

	Super::InitRemoteConnection(InDriver, InSocket, InURL, InRemoteAddr, InState, InMaxPacket, InPacketOverhead);
	if (!bIsPassthrough && RemoteAddr.IsValid())
	{
		//FSocketSubsystemEscape* SocketSubsystem = (FSocketSubsystemEscape*)ISocketSubsystem::Get(ESCAPE_SUBSYSTEM);
		//if (SocketSubsystem)
		//{
		//	//SocketSubsystem->RegisterConnection(this);
		//}
	}
}

void UEscapeNetConnection::CleanUp()
{
	Super::CleanUp();

	if (!bIsPassthrough)
	{
		FSocketSubsystemEscape* SocketSubsystem = (FSocketSubsystemEscape*)ISocketSubsystem::Get(ESCAPE_SUBSYSTEM);
		if (SocketSubsystem)
		{
			// Unregister the connection AFTER the parent class has had a chance to close/flush connections
			//SocketSubsystem->UnregisterConnection(this);
		}
	}
}

