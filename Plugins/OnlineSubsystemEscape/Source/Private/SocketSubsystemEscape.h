// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/WeakObjectPtr.h"
#include "SocketSubsystem.h"
#include "IPAddress.h"
#include "Containers/Ticker.h"
#include "OnlineSubsystemEscapePackage.h"

class Error;

class FSocketSubsystemEscape :
	public ISocketSubsystem, public FTickerObjectBase
{
protected:
	/** Single instantiation of this subsystem */
	static FSocketSubsystemEscape* SocketSingleton;

	/** Tracks existing Steamworks sockets, for connection failure/timeout resolution */
	TArray<class FSocket*> EscapeSockets;

	/**
	* Adds a escape socket for tracking
	*
	* @param InSocket	The socket to add for tracking
	*/
	void AddSocket(class FSocket* InSocket)
	{
		EscapeSockets.Add(InSocket);
	}

	/**
	* Removes a escape socket from tracking
	*
	* @param InSocket	The socket to remove from tracking
	*/
	void RemoveSocket(class FSocket* InSocket)
	{
		EscapeSockets.RemoveSingleSwap(InSocket);
	}

PACKAGE_SCOPE:

	/** Last error set by the socket subsystem or one of its sockets */
	int32 LastSocketError;

	/**
	* Singleton interface for this subsystem
	* @return the only instance of this subsystem
	*/
	static FSocketSubsystemEscape* Create();

	/**
	* Performs Windows specific socket clean up
	*/
	static void Destroy();

public:
	FSocketSubsystemEscape() :
		LastSocketError(0)
	{
	}
	/**
	* Does Windows platform initialization of the sockets library
	*
	* @param Error a string that is filled with error information
	*
	* @return true if initialized ok, false otherwise
	*/
	virtual bool Init(FString& Error) override;

	/**
	* Performs platform specific socket clean up
	*/
	virtual void Shutdown() override;

	/**
	* Creates a socket
	*
	* @Param SocketType type of socket to create (DGram, Stream, etc)
	* @param SocketDescription debug description
	* @param bForceUDP overrides any platform specific protocol with UDP instead
	*
	* @return the new socket or NULL if failed
	*/
	virtual class FSocket* CreateSocket(const FName& SocketType, const FString& SocketDescription, bool bForceUDP = false) override;

	/**
	* Cleans up a socket class
	*
	* @param Socket the socket object to destroy
	*/
	virtual void DestroySocket(class FSocket* Socket) override;

	/**
	* Does a DNS look up of a host name
	*
	* @param HostName the name of the host to look up
	* @param OutAddr the address to copy the IP address to
	*/
	virtual ESocketErrors GetHostByName(const ANSICHAR* HostName, FInternetAddr& OutAddr) override;

	/**
	* Some platforms require chat data (voice, text, etc.) to be placed into
	* packets in a special way. This function tells the net connection
	* whether this is required for this platform
	*/
	virtual bool RequiresChatDataBeSeparate() override
	{
		return false;
	}

	/**
	* Some platforms require packets be encrypted. This function tells the
	* net connection whether this is required for this platform
	*/
	virtual bool RequiresEncryptedPackets() override
	{
		return false;
	}

	/**
	* Determines the name of the local machine
	*
	* @param HostName the string that receives the data
	*
	* @return true if successful, false otherwise
	*/
	virtual bool GetHostName(FString& HostName) override;

	/**
	*	Create a proper FInternetAddr representation
	* @param Address host address
	* @param Port host port
	*/
	virtual TSharedRef<FInternetAddr> CreateInternetAddr(uint32 Address = 0, uint32 Port = 0) override;

	/**
	* @return Whether the machine has a properly configured network device or not
	*/
	virtual bool HasNetworkDevice() override;

	/**
	*	Get the name of the socket subsystem
	* @return a string naming this subsystem
	*/
	virtual const TCHAR* GetSocketAPIName() const override;

	/**
	* Returns the last error that has happened
	*/
	virtual ESocketErrors GetLastErrorCode() override;

	/**
	* Translates the platform error code to a ESocketErrors enum
	*/
	virtual ESocketErrors TranslateErrorCode(int32 Code) override;

	/**
	* Gets the list of addresses associated with the adapters on the local computer.
	*
	* @param OutAdresses - Will hold the address list.
	*
	* @return true on success, false otherwise.
	*/
	virtual bool GetLocalAdapterAddresses(TArray<TSharedPtr<FInternetAddr> >& OutAdresses) override
	{
		bool bCanBindAll;

		OutAdresses.Add(GetLocalHostAddr(*GLog, bCanBindAll));

		return true;
	}

	/**
	* Chance for the socket subsystem to get some time
	*
	* @param DeltaTime time since last tick
	*/
	virtual bool Tick(float DeltaTime) override;
};

/**
 * Create the socket subsystem for the given platform service
 */
FName CreateSteamSocketSubsystem();