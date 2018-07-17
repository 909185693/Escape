// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SocketSubsystemEscape.h"
#include "Misc/ConfigCacheIni.h"
#include "SocketsEscape.h"
#include "IPAddress.h"
#include "OnlineSessionInterface.h"
#include "SocketSubsystemModule.h"


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
	return nullptr;
}

/**
* Cleans up a socket class
*
* @param Socket the socket object to destroy
*/
void FSocketSubsystemEscape::DestroySocket(FSocket* Socket)
{
	// Possible non steam socket here PLATFORM_SOCKETSUBSYSTEM, but its just a pointer compare
	RemoveSocket((FSocketsEscape*)Socket);
	delete Socket;
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
	FInternetAddr* InternetAddr = nullptr;
	return MakeShareable(InternetAddr);
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

/**
*	Get local IP to bind to
*/
TSharedRef<FInternetAddr> FSocketSubsystemEscape::GetLocalBindAddr(FOutputDevice& Out)
{
	FInternetAddr* InternetAddr = nullptr;
	return MakeShareable(InternetAddr);
}

bool FSocketSubsystemEscape::Tick(float DeltaTime)
{
	return true;
}
