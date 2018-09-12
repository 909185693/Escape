// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemTypes.h"
#include "IPAddress.h"

#ifndef ESCAPE_SUBSYSTEM
#define ESCAPE_SUBSYSTEM TEXT("ESCAPE")
#endif

#ifndef ESCAPE_URL_PREFIX
#define ESCAPE_URL_PREFIX TEXT("escape.")
#endif

#ifndef ESCAPE_SOCKET_TYPE_CLIENT
#define ESCAPE_SOCKET_TYPE_CLIENT FName(TEXT("EscapeSocketClient"))
#endif

#ifndef ESCAPE_SOCKET_TYPE_SERVER
#define ESCAPE_SOCKET_TYPE_SERVER FName(TEXT("EscapeSocketServer"))
#endif

#ifndef ESCAPE_BUILD_LOGINSERVER
#define ESCAPE_BUILD_LOGINSERVER UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT || UE_SERVER
#endif

class FOnlineSubsystemEscape;

// from OnlineSubsystemTypes.h
TEMP_UNIQUENETIDSTRING_SUBCLASS(FUniqueNetIdEscape, ESCAPE_SUBSYSTEM);

/** 
 * Implementation of session information
 */
class FOnlineSessionInfoEscape : public FOnlineSessionInfo
{
protected:
	
	/** Hidden on purpose */
	FOnlineSessionInfoEscape(const FOnlineSessionInfoEscape& Src)
	{
	}

	/** Hidden on purpose */
	FOnlineSessionInfoEscape& operator=(const FOnlineSessionInfoEscape& Src)
	{
		return *this;
	}

PACKAGE_SCOPE:

	/** Constructor */
	FOnlineSessionInfoEscape();

	/** 
	 * Initialize a Null session info with the address of this machine
	 * and an id for the session
	 */
	void Init(const FOnlineSubsystemEscape& Subsystem);

	/** The ip & port that the host is listening on (valid for LAN/GameServer) */
	TSharedPtr<class FInternetAddr> HostAddr;

	/** Unique Id for this session */
	FUniqueNetIdEscape SessionId;

public:

	virtual ~FOnlineSessionInfoEscape() {}

 	bool operator==(const FOnlineSessionInfoEscape& Other) const
 	{
 		return false;
 	}

	virtual const uint8* GetBytes() const override
	{
		return NULL;
	}

	virtual int32 GetSize() const override
	{
		return sizeof(uint64) + sizeof(TSharedPtr<class FInternetAddr>);
	}

	virtual bool IsValid() const override
	{
		// LAN case
		return HostAddr.IsValid() && HostAddr->IsValid();
	}

	virtual FString ToString() const override
	{
		return SessionId.ToString();
	}

	virtual FString ToDebugString() const override
	{
		return FString::Printf(TEXT("HostIP: %s SessionId: %s"), 
			HostAddr.IsValid() ? *HostAddr->ToString(true) : TEXT("INVALID"), 
			*SessionId.ToDebugString());
	}

	virtual const FUniqueNetId& GetSessionId() const override
	{
		return SessionId;
	}
};



struct FUserLogin
{
	char UserName[20];
	char PassWord[20];
}
