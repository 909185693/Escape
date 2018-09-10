// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "EscapeNetConnection.h"
#include "OnlineSubsystemNames.h"
#include "OnlineSubsystem.h"
#include "SocketSubsystem.h"
#include "SocketSubsystemEscape.h"
#include "EscapeNetDriver.h"
#include "OnlineSubsystemEscapeTypes.h"


UEscapeNetConnection::UEscapeNetConnection(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
}

void UEscapeNetConnection::InitLocalConnection(UNetDriver* InDriver, class FSocket* InSocket, const FURL& InURL, EConnectionState InState, int32 InMaxPacket, int32 InPacketOverhead)
{
	Super::InitLocalConnection(InDriver, InSocket, InURL, InState, InMaxPacket, InPacketOverhead);
}

void UEscapeNetConnection::InitRemoteConnection(UNetDriver* InDriver, class FSocket* InSocket, const FURL& InURL, const class FInternetAddr& InRemoteAddr, EConnectionState InState, int32 InMaxPacket, int32 InPacketOverhead)
{
	Super::InitRemoteConnection(InDriver, InSocket, InURL, InRemoteAddr, InState, InMaxPacket, InPacketOverhead);
}

void UEscapeNetConnection::CleanUp()
{
	Super::CleanUp();
}

