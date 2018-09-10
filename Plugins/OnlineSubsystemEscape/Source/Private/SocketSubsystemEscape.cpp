// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SocketSubsystemEscape.h"
#include "Misc/ConfigCacheIni.h"
#include "IPAddress.h"
#include "OnlineSessionInterface.h"
#include "SocketSubsystemModule.h"
#include "OnlineSubsystemEscapeTypes.h"

/**
* Create the socket subsystem for the given platform service
*/
FName CreateSteamSocketSubsystem()
{
	// Create and register our singleton factory with the main online subsystem for easy access
	FSocketSubsystemEscape* SocketSubsystem = FSocketSubsystemEscape::Create();
	FString Error;
	if (SocketSubsystem->Init(Error))
	{
		FSocketSubsystemModule& SSS = FModuleManager::LoadModuleChecked<FSocketSubsystemModule>("Sockets");
		SSS.RegisterSocketSubsystem(ESCAPE_SUBSYSTEM, SocketSubsystem, true);
		return ESCAPE_SUBSYSTEM;
	}
	else
	{
		FSocketSubsystemEscape::Destroy();
		return NAME_None;
	}
}


FSocketSubsystemEscape* FSocketSubsystemEscape::SocketSingleton = nullptr;

/**
* Singleton interface for this subsystem
* @return the only instance of this subsystem
*/
FSocketSubsystemEscape* FSocketSubsystemEscape::Create()
{
	if (SocketSingleton == NULL)
	{
		SocketSingleton = new FSocketSubsystemEscape();
	}

	return SocketSingleton;
}

void FSocketSubsystemEscape::Destroy()
{
	if (SocketSingleton != NULL)
	{
		SocketSingleton->Shutdown();
		delete SocketSingleton;
		SocketSingleton = NULL;
	}
}

/**
* Does Steam platform initialization of the sockets library
*
* @param Error a string that is filled with error information
*
* @return true if initialized ok, false otherwise
*/
bool FSocketSubsystemEscape::Init(FString& Error)
{
	return true;
}

/**
* Performs platform specific socket clean up
*/
void FSocketSubsystemEscape::Shutdown()
{

}

/**
* Creates a socket
*
* @Param SocketType type of socket to create (DGram, Stream, etc)
* @param SocketDescription debug description
* @param bForceUDP overrides any platform specific protocol with UDP instead
*
* @return the new socket or NULL if failed
*/
FSocket* FSocketSubsystemEscape::CreateSocket(const FName& SocketType, const FString& SocketDescription, bool bForceUDP)
{
	FSocket* NewSocket = NULL;

	ISocketSubsystem* PlatformSocketSub = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	if (PlatformSocketSub)
	{
		NewSocket = PlatformSocketSub->CreateSocket(SocketType, SocketDescription, bForceUDP);

		AddSocket(NewSocket);
	}

	return NewSocket;
}

/**
* Cleans up a socket class
*
* @param Socket the socket object to destroy
*/
void FSocketSubsystemEscape::DestroySocket(FSocket* Socket)
{
	// Possible non steam socket here PLATFORM_SOCKETSUBSYSTEM, but its just a pointer compare
	RemoveSocket(Socket);
	
	ISocketSubsystem* PlatformSocketSub = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	if (PlatformSocketSub)
	{
		PlatformSocketSub->DestroySocket(Socket);
	}
}

/**
* Does a DNS look up of a host name
*
* @param HostName the name of the host to look up
* @param OutAddr the address to copy the IP address to
*/
ESocketErrors FSocketSubsystemEscape::GetHostByName(const ANSICHAR* HostName, FInternetAddr& OutAddr)
{
	return SE_EADDRNOTAVAIL;
}

/**
* Determines the name of the local machine
*
* @param HostName the string that receives the data
*
* @return true if successful, false otherwise
*/
bool FSocketSubsystemEscape::GetHostName(FString& HostName)
{
	return false;
}

/**
*	Create a proper FInternetAddr representation
* @param Address host address
* @param Port host port
*/
TSharedRef<FInternetAddr> FSocketSubsystemEscape::CreateInternetAddr(uint32 Address, uint32 Port)
{
	return ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();;
}

/**
* @return Whether the machine has a properly configured network device or not
*/
bool FSocketSubsystemEscape::HasNetworkDevice()
{
	return true;
}

/**
*	Get the name of the socket subsystem
* @return a string naming this subsystem
*/
const TCHAR* FSocketSubsystemEscape::GetSocketAPIName() const
{
	return TEXT("EscapeSockets");
}

/**
* Returns the last error that has happened
*/
ESocketErrors FSocketSubsystemEscape::GetLastErrorCode()
{
	return TranslateErrorCode(LastSocketError);
}

/**
* Translates the platform error code to a ESocketErrors enum
*/
ESocketErrors FSocketSubsystemEscape::TranslateErrorCode(int32 Code)
{
	// @TODO ONLINE - This needs to be filled in (at present it is 1:1)
	return (ESocketErrors)LastSocketError;
}

bool FSocketSubsystemEscape::Tick(float DeltaTime)
{
	return true;
}
