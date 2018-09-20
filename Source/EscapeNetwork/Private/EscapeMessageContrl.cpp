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
		EscapeServer->AddToParallelTasks(this);
		EscapeServer->AddMessageCallback(ELogicCode::USER_LOGIN, this, &UEscapeMessageContrl::NotifyUserLogin);
		EscapeServer->AddMessageCallback(ELogicCode::MATCH_GAME, this, &UEscapeMessageContrl::NotifyMatchGame);
		EscapeServer->AddMessageCallback(ELogicCode::CANCEL_MATCH, this, &UEscapeMessageContrl::NotifyCancelGame);
		EscapeServer->AddMessageCallback(ELogicCode::INVITATION, this, &UEscapeMessageContrl::NotifyInvitation);
		EscapeServer->AddMessageCallback(ELogicCode::CONNECTION, this, &UEscapeMessageContrl::NotifyConnection);
		EscapeServer->AddMessageCallback(ELogicCode::REGISTER_SERVER, this, &UEscapeMessageContrl::NotifyRegisterServer);
		EscapeServer->AddMessageCallback(ELogicCode::MATCH_STATE, this, &UEscapeMessageContrl::NotifyMatchState);
		EscapeServer->AddMessageCallback(ELogicCode::NUMPLAYERS, this, &UEscapeMessageContrl::NotifyNumPlayers);
	}
}

void UEscapeMessageContrl::BeginDestroy()
{
	Super::BeginDestroy();
}

void UEscapeMessageContrl::NotifyConnection(TSharedPtr<FConnection> Connection, void* Data, EErrorCode Error)
{
	if (Error != EErrorCode::NONE)
	{
		MatchConnections.Remove(Connection);
		PendingMatchConnections.Remove(Connection);

		TSharedPtr<FDedicatedServerInfo> DedicatedServerInfo = StaticCastSharedPtr<FDedicatedServerInfo>(Connection);
		if (DedicatedServerInfo.IsValid())
		{
			DedicatedServerInfoSessions.Remove(DedicatedServerInfo);
		}

		UE_LOG(LogEscapeNetwork, Log, TEXT("EscapeServer : Disconnect error MatchConnections[%d] DedicatedServerInfoSessions[%d]"), MatchConnections.Num(), DedicatedServerInfoSessions.Num());
	}
}

void UEscapeMessageContrl::NotifyUserLogin(TSharedPtr<FConnection> Connection, void* Data, EErrorCode Error)
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
			EscapeServer->SendTo(Connection->Socket, ELogicCode::USER_LOGIN, EErrorCode::NONE, sizeof(FUser), &SendData);
		}
		else
		{
			EscapeServer->SendTo(Connection->Socket, ELogicCode::USER_LOGIN, EErrorCode::PASSWORD_ERROR);
		}
	}
}

void UEscapeMessageContrl::NotifyMatchGame(TSharedPtr<FConnection> Connection, void* Data, EErrorCode Error)
{
	MatchConnections.AddUnique(Connection);
}

void UEscapeMessageContrl::NotifyCancelGame(TSharedPtr<FConnection> Connection, void* Data, EErrorCode Error)
{
	MatchConnections.Remove(Connection);
	PendingMatchConnections.Remove(Connection);
}

void UEscapeMessageContrl::NotifyInvitation(TSharedPtr<FConnection> Connection, void* Data, EErrorCode Error)
{

}

void UEscapeMessageContrl::NotifyRegisterServer(TSharedPtr<FConnection> Connection, void* Data, EErrorCode Error)
{
	bool bRegisterSuccess = false;

	if (Error == EErrorCode::NONE)
	{
		FDedicatedServer* DedicatedServer = (FDedicatedServer*)Data;
		for (TSharedPtr<FDedicatedServerInfo> DedicatedServerInfo : DedicatedServerInfoSessions)
		{
			if (DedicatedServerInfo->Guid == FString(ANSI_TO_TCHAR(DedicatedServer->Guid)))
			{
				bRegisterSuccess = EscapeServer->RegisterConnection(Connection, DedicatedServerInfo);
				if (bRegisterSuccess)
				{
					DedicatedServerInfo->bIsRegister = true;
					DedicatedServerInfo->Port = DedicatedServer->Port;
					DedicatedServerInfo->MinPlayers = DedicatedServer->MinPlayers;
					DedicatedServerInfo->MaxPlayers = DedicatedServer->MaxPlayers;

					TSharedPtr<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
					Connection->Socket->GetPeerAddress(*InternetAddr);
					DedicatedServerInfo->IP = InternetAddr->ToString(false);

					UE_LOG(LogEscapeNetwork, Log, TEXT("EscapeServer : Register success dedicated server %s:%d MinPlayers[%d] MaxPlayers[%d]"),
						*DedicatedServerInfo->IP,
						DedicatedServerInfo->Port,
						DedicatedServerInfo->MinPlayers,
						DedicatedServerInfo->MaxPlayers);
				}
			}
		}
	}

	if (!bRegisterSuccess)
	{
		EscapeServer->CloseConnection(Connection);
	}
}

void UEscapeMessageContrl::NotifyMatchState(TSharedPtr<FConnection> Connection, void* Data, EErrorCode Error)
{
	TSharedPtr<FDedicatedServerInfo> DedicatedServerInfo = StaticCastSharedPtr<FDedicatedServerInfo>(Connection);
	if (DedicatedServerInfo.IsValid())
	{
		DedicatedServerInfo->MatchState = *(EMatchState*)Data;

		UE_LOG(LogEscapeNetwork, Log, TEXT("EscapeServer : Sync game state [%d]"), (int32)DedicatedServerInfo->MatchState);
	}
}

void UEscapeMessageContrl::NotifyNumPlayers(TSharedPtr<FConnection> Connection, void* Data, EErrorCode Error)
{
	TSharedPtr<FDedicatedServerInfo> DedicatedServerInfo = StaticCastSharedPtr<FDedicatedServerInfo>(Connection);
	if (DedicatedServerInfo.IsValid())
	{
		DedicatedServerInfo->NumPlayers = *(int32*)Data;

		UE_LOG(LogEscapeNetwork, Log, TEXT("EscapeServer : Sync num players [%d]"), (int32)DedicatedServerInfo->NumPlayers);
	}
}

void UEscapeMessageContrl::ClientTravel()
{
	const int32 MinStartupPlayers = 2;
	if (MatchConnections.Num() >= MinStartupPlayers)
	{
		StartupDedicatedServer();

		TArray<TSharedPtr<FConnection>>::TIterator It(MatchConnections);
		for (int32 Index = 0; Index < MinStartupPlayers && It; ++Index, ++It)
		{
			PendingMatchConnections.AddUnique(*It);

			It.RemoveCurrent();
		}
	}

	for (TSharedPtr<FDedicatedServerInfo> DedicatedServerInfo : DedicatedServerInfoSessions)
	{
		/// 服务器状态
		if (DedicatedServerInfo->MatchState != EMatchState::EnteringMap)
		{
			continue;
		}

		/// 服务器注册超时
		if (!DedicatedServerInfo->bIsRegister)
		{
			double CurrentTime = FPlatformTime::Seconds();
			if (CurrentTime - DedicatedServerInfo->StartTime > 60)
			{
				EscapeServer->CloseConnection(DedicatedServerInfo);
			}
			continue;
		}

		/// 匹配玩家
		if (DedicatedServerInfo->bIsRegister &&
			DedicatedServerInfo->NumPlayers < DedicatedServerInfo->MaxPlayers &&
			DedicatedServerInfo->NumPlayers + MatchConnections.Num() + PendingMatchConnections.Num() >= DedicatedServerInfo->MinPlayers)
		{
			FClientTravel Travel;
			FString URL = FString::Printf(TEXT("%s:%d"), *DedicatedServerInfo->IP, DedicatedServerInfo->Port);
			FPlatformString::Strncpy(Travel.URL, TCHAR_TO_ANSI(*URL), sizeof(Travel.URL));

			for (TArray<TSharedPtr<FConnection>>::TIterator It(PendingMatchConnections) ; It; ++It)
			{
				TSharedPtr<FConnection> ClientConnection = *It;
				if (ClientConnection.IsValid())
				{
					It.RemoveCurrent();
					
					EscapeServer->SendTo(ClientConnection->Socket, ELogicCode::CLIENT_TRAVEL, EErrorCode::NONE, sizeof(FClientTravel), &Travel);
					
					if (++DedicatedServerInfo->NumPlayers >= DedicatedServerInfo->MaxPlayers)
					{
						break;
					}
				}
			}

			for (TArray<TSharedPtr<FConnection>>::TIterator It(MatchConnections); It; ++It)
			{
				TSharedPtr<FConnection> ClientConnection = *It;
				if (ClientConnection.IsValid())
				{
					It.RemoveCurrent();

					EscapeServer->SendTo(ClientConnection->Socket, ELogicCode::CLIENT_TRAVEL, EErrorCode::NONE, sizeof(FClientTravel), &Travel);

					if (++DedicatedServerInfo->NumPlayers >= DedicatedServerInfo->MaxPlayers)
					{
						break;
					}
				}
			}
		}
	}
}

bool UEscapeMessageContrl::StartupDedicatedServer()
{
	FString GamePath;
	FString Params;
	FGuid Guid = FGuid::NewGuid();
	FString MapName(TEXT("EscapeMap"));
	FString GameMode(TEXT("Game"));

#if UE_BUILD_SHIPPING || UE_SERVER
	int32 CurrentProcessId = FPlatformProcess::GetCurrentProcessId();
	GamePath = FPlatformProcess::GetApplicationName(CurrentProcessId);
	Params = FString::Printf(TEXT("%s?game=%s?guid=%s -server"), *MapName, *GameMode, *Guid.ToString());
#else
	FString GameNameOrProjectFile;
	if (FPaths::IsProjectFilePathSet())
	{
		GameNameOrProjectFile = FString::Printf(TEXT("\"%s\""), *FPaths::GetProjectFilePath());
	}
	else
	{
		GameNameOrProjectFile = FApp::GetProjectName();
	}
	GamePath = FPlatformProcess::GenerateApplicationPath(FApp::GetName(), FApp::GetBuildConfiguration());
	Params = FString::Printf(TEXT("%s %s?game=%s?guid=%s -server -log"), *GameNameOrProjectFile, *MapName, *GameMode, *Guid.ToString());
#endif
	TSharedPtr<FDedicatedServerInfo> NewSession = MakeShareable(new FDedicatedServerInfo());

	uint32 ProcessID = 0;
	NewSession->ProcessHandle = FPlatformProcess::CreateProc(*GamePath, *Params, true, false, false, &ProcessID, 0, NULL, NULL);
	NewSession->ProcessID = ProcessID;
	NewSession->Guid = Guid.ToString();
	NewSession->StartTime = FPlatformTime::Seconds();

	DedicatedServerInfoSessions.Add(NewSession);

	bool bStartup = NewSession->ProcessHandle.IsValid();
	if (bStartup)
	{
		UE_LOG(LogEscapeNetwork, Log, TEXT("Run a dedicated server of the game. Guid[%s] MapName[%s] GameMode[%s] StartTime[%f]"), *Guid.ToString(), *MapName, *GameMode, NewSession->StartTime);
	}
	else
	{
		UE_LOG(LogEscapeNetwork, Error, TEXT("Failed to run a dedicated server of the game."));
	}

	return bStartup;
}

bool UEscapeMessageContrl::IsDone() const
{
	return HasAnyFlags(RF_BeginDestroyed);
}

bool UEscapeMessageContrl::WasSuccessful() const
{
	return true;
}

void UEscapeMessageContrl::Tick()
{
	// assert that we're not on the game thread
	check(!IsInGameThread() || !FPlatformProcess::SupportsMultithreading());

	ClientTravel();
}

FString UEscapeMessageContrl::ToString() const
{
	return TEXT("EscapeMessageContrlTask");
}