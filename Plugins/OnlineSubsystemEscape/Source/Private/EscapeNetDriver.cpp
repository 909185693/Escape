// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "EscapeNetDriver.h"
#include "OnlineSubsystemNames.h"
#include "OnlineSubsystem.h"
#include "SocketSubsystem.h"
#include "OnlineSubsystemEscape.h"
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

void UEscapeNetDriver::TickDispatch(float DeltaTime)
{
	Super::TickDispatch(DeltaTime);
}

bool UEscapeNetDriver::InitBase(bool bInitAsClient, FNetworkNotify* InNotify, const FURL& URL, bool bReuseAddressAndPort, FString& Error)
{
	bool bResult = Super::InitBase(bInitAsClient, InNotify, URL, bReuseAddressAndPort, Error);

	if (bResult)
	{
		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
		if (SocketSubsystem == nullptr)
		{
			UE_LOG(LogNet, Warning, TEXT("Unable to find socket subsystem"));
			return false;
		}

		FOnlineSubsystemEscape* OnlineSubsystem = (FOnlineSubsystemEscape*)IOnlineSubsystem::Get(ESCAPE_SUBSYSTEM);
		if (OnlineSubsystem == nullptr)
		{
			UE_LOG(LogNet, Warning, TEXT("Unable to find online subsystem"));
			return false;
		}

		OnlineSubsystem->Socket = SocketSubsystem->CreateSocket(NAME_Stream, ESCAPE_SUBSYSTEM);
		OnlineSubsystem->Socket->SetNonBlocking();

		if (OnlineSubsystem->Socket == nullptr)
		{
			OnlineSubsystem->Socket = 0;
			Error = FString::Printf(TEXT("WinSock: socket failed (%i)"), (int32)SocketSubsystem->GetLastErrorCode());
			return false;
		}
	}

	return bResult;
}

bool UEscapeNetDriver::InitConnect(FNetworkNotify* InNotify, const FURL& ConnectURL, FString& Error)
{
	bool bResult = Super::InitConnect(InNotify, ConnectURL, Error);

	if (bResult)
	{
		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
		if (SocketSubsystem == nullptr)
		{
			UE_LOG(LogNet, Warning, TEXT("Unable to find socket subsystem"));
			return false;
		}

		TSharedPtr<FInternetAddr> HostAddr = SocketSubsystem->CreateInternetAddr();
		bool bIsValid = false;
		HostAddr->SetIp(*ConnectURL.Host, bIsValid);
		HostAddr->SetPort(EscapeServerPort);

		FOnlineSubsystemEscape* OnlineSubsystem = (FOnlineSubsystemEscape*)IOnlineSubsystem::Get(ESCAPE_SUBSYSTEM);
		if (OnlineSubsystem == nullptr)
		{
			UE_LOG(LogNet, Warning, TEXT("Unable to find online subsystem"));
			return false;
		}

		if (!OnlineSubsystem->Socket->Connect(*HostAddr))
		{
			UE_LOG(LogNet, Warning, TEXT("Connect to escape logic server failed. HostAddr [%s]"), *HostAddr->ToString(true));
			return false;
		}

		FUserLogin UserLogin;
		FPlatformString::Strcpy(UserLogin.UserName, 9, "909185693");
		FPlatformString::Strcpy(UserLogin.PassWord, 9, "123456789");

		SendTo(OnlineSubsystem->Socket, ELoginCode::USER_LOGIN, sizeof(FUserLogin), &UserLogin);
	}

	return bResult;
}

bool UEscapeNetDriver::InitListen(FNetworkNotify* InNotify, FURL& ListenURL, bool bReuseAddressAndPort, FString& Error)
{
	bool bResult = Super::InitListen(InNotify, ListenURL, bReuseAddressAndPort, Error);

#if ESCAPE_BUILD_LOGINSERVER
	if (bResult && ListenURL.HasOption(TEXT("IsLogicServer")))
	{
		TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		InternetAddr->SetPort(EscapeServerPort);
		
		FOnlineSubsystemEscape* OnlineSubsystem = (FOnlineSubsystemEscape*)IOnlineSubsystem::Get(ESCAPE_SUBSYSTEM);
		if (OnlineSubsystem == nullptr)
		{
			UE_LOG(LogNet, Warning, TEXT("Unable to find online subsystem"));
			return false;
		}
		check(OnlineSubsystem->Socket);
		if (!OnlineSubsystem->Socket->Bind(InternetAddr.Get()))
		{
			Error = FString::Printf(TEXT("Failed to bind EscapeSocket on port[%d]"), EscapeServerPort);
			return false;
		}

		if (!OnlineSubsystem->Socket->Listen(EscapeMaxBackLog))
		{
			Error = FString::Printf(TEXT("Failed to listen EscapeSocket on port[%d]"), EscapeServerPort);
			return false;
		}

		OnlineSubsystem->SetLogicServer(true);

		UE_LOG(LogNet, Log, TEXT("EscapeSocket listening on port %i"), EscapeServerPort);
	}
#endif

	return bResult;
}

void UEscapeNetDriver::Shutdown()
{
	Super::Shutdown();
}