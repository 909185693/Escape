// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "EscapeMessageContrl.h"
#include "EscapeServer.h"
#include "Misc/App.h"


UEscapeMessageContrl::UEscapeMessageContrl(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, EscapeServer(nullptr)
{

}

void UEscapeMessageContrl::Register(UEscapeServer* InEscapeServer)
{
	EscapeServer = InEscapeServer;
	if (EscapeServer != nullptr)
	{
		EscapeServer->AddMessageCallback(ELogicCode::USER_LOGIN, this, &UEscapeMessageContrl::NotifyUserLogin);
		EscapeServer->AddMessageCallback(ELogicCode::MATCH_GAME, this, &UEscapeMessageContrl::NotifyMatchGame);
		EscapeServer->AddMessageCallback(ELogicCode::INVITATION, this, &UEscapeMessageContrl::NotifyInvitation);
		EscapeServer->AddMessageCallback(ELogicCode::CONNECTION, this, &UEscapeMessageContrl::NotifyConnection);
		EscapeServer->AddMessageCallback(ELogicCode::REGISTER_SERVER, this, &UEscapeMessageContrl::NotifyRegisterServer);
	}
}

void UEscapeMessageContrl::BeginDestroy()
{
	Super::BeginDestroy();
}

void UEscapeMessageContrl::NotifyConnection(FConnection& Connection, void* Data, EErrorCode Error)
{
	if (Error != EErrorCode::NONE)
	{
		MatchConnections.Remove(Connection);
	}
}

void UEscapeMessageContrl::NotifyUserLogin(FConnection& Connection, void* Data, EErrorCode Error)
{
	FUserLogin* UserLogin = (FUserLogin*)Data;
	if (UserLogin != nullptr)
	{
		if (FPlatformString::Strncmp(ANSI_TO_TCHAR(UserLogin->Username), TEXT("909185693"), sizeof(UserLogin->Username)) == 0 &&
			FPlatformString::Strncmp(ANSI_TO_TCHAR(UserLogin->Password), TEXT("111111"), sizeof(UserLogin->Password)) == 0)
		{
			FUser SendData;
			SendData.ID = 10086;
			FPlatformString::Strncpy(SendData.Nickname, TCHAR_TO_UTF8(TEXT("中文测试Test")), sizeof(SendData.Nickname));
			EscapeServer->SendTo(*Connection, ELogicCode::USER_LOGIN, EErrorCode::NONE, sizeof(FUser), &SendData);
		}
		else
		{
			EscapeServer->SendTo(*Connection, ELogicCode::USER_LOGIN, EErrorCode::PASSWORD_ERROR);
		}
	}
}

void UEscapeMessageContrl::NotifyMatchGame(FConnection& Connection, void* Data, EErrorCode Error)
{
	bool bResult = false;

	MatchConnections.AddUnique(Connection);

	if (DedicatedServerInfoSessions.Num() == 0)
	{
		FString GameNameOrProjectFile;

#if UE_BUILD_SHIPPING
		int32 CurrentProcessId = FPlatformProcess::GetCurrentProcessId();
		GameNameOrProjectFile = FPlatformProcess::GetApplicationName(CurrentProcessId);
#else
		if (FPaths::IsProjectFilePathSet())
		{
			GameNameOrProjectFile = FString::Printf(TEXT("\"%s\""), *FPaths::GetProjectFilePath());
		}
		else
		{
			GameNameOrProjectFile = FApp::GetProjectName();
		}
#endif

		FGuid Guid = FGuid::NewGuid();
		FString MapName(TEXT("EscapeMap"));
		FString GameMode(TEXT("Game"));
		FString Params = FString::Printf(TEXT("%s %s?game=%s?guid=%s -server -log"), *GameNameOrProjectFile, *MapName, *GameMode, *Guid.ToString());

		FString GamePath = FPlatformProcess::GenerateApplicationPath(FApp::GetName(), FApp::GetBuildConfiguration());
		FDedicatedServerInfo* NewSession = new (DedicatedServerInfoSessions) FDedicatedServerInfo();

		uint32 ProcessID = 0;
		NewSession->ProcessHandle = FPlatformProcess::CreateProc(*GamePath, *Params, true, false, false, &ProcessID, 0, NULL, NULL);
		NewSession->ProcessID = ProcessID;
		NewSession->Guid = Guid.ToString();

		if (NewSession->ProcessHandle.IsValid())
		{
			bResult = true;

			UE_LOG(LogEscapeNetwork, Log, TEXT("Run a dedicated server of the game. Guid[%s] MapName[%s] GameMode[%s]"), *Guid.ToString(), *MapName, *GameMode);
		}
		else
		{
			UE_LOG(LogEscapeNetwork, Error, TEXT("Failed to run a dedicated server of the game."));
		}
	}

	EscapeServer->SendTo(*Connection, ELogicCode::MATCH_GAME, (bResult || ClientTravel()) ? EErrorCode::NONE : EErrorCode::UNKNOWN_ERROR);
}

void UEscapeMessageContrl::NotifyInvitation(FConnection& Connection, void* Data, EErrorCode Error)
{

}

void UEscapeMessageContrl::NotifyRegisterServer(FConnection& Connection, void* Data, EErrorCode Error)
{
	bool bRegisterSuccess = false;

	if (Error == EErrorCode::NONE)
	{
		FDedicatedServer* DedicatedServer = (FDedicatedServer*)Data;
		for (FDedicatedServerInfo& DedicatedServerInfo : DedicatedServerInfoSessions)
		{
			if (DedicatedServerInfo.Guid == FString(ANSI_TO_TCHAR(DedicatedServer->Guid)))
			{
				bRegisterSuccess = true;

				DedicatedServerInfo.bIsRegister = true;
				DedicatedServerInfo.Port = DedicatedServer->Port;

				TSharedPtr<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
				Connection->GetPeerAddress(*InternetAddr);
				DedicatedServerInfo.IP = InternetAddr->ToString(false);

				UE_LOG(LogEscapeNetwork, Log, TEXT("EscapeServer : Register dedicated server %s:%d"), *DedicatedServerInfo.IP, DedicatedServerInfo.Port);
			}
		}

		ClientTravel();
	}

	if (!bRegisterSuccess)
	{
		EscapeServer->CloseConnection(Connection);

		for (FConnection& ClientConnection : MatchConnections)
		{
			EscapeServer->SendTo(*ClientConnection, ELogicCode::MATCH_GAME, EErrorCode::UNKNOWN_ERROR);
		}

		MatchConnections.Empty();
	}
}

bool UEscapeMessageContrl::ClientTravel()
{
	for (FDedicatedServerInfo& DedicatedServerInfo : DedicatedServerInfoSessions)
	{
		if (DedicatedServerInfo.bIsRegister)
		{
			FClientTravel Travel;
			FString URL = FString::Printf(TEXT("%s:%d"), *DedicatedServerInfo.IP, DedicatedServerInfo.Port);
			FPlatformString::Strncpy(Travel.URL, TCHAR_TO_ANSI(*URL), sizeof(Travel.URL));
			for (FConnection& ClientConnection : MatchConnections)
			{
				EscapeServer->SendTo(*ClientConnection, ELogicCode::CLIENT_TRAVEL, EErrorCode::NONE, sizeof(FClientTravel), &Travel);
			}

			MatchConnections.Empty();

			return true;
		}
	}

	return false;
}