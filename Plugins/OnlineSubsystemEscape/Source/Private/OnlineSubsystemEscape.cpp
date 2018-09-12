// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "OnlineSubsystemEscape.h"
#include "OnlineAsyncTaskManagerEscape.h"
#include "OnlineSessionInterfaceEscape.h"
#include "OnlineExternalUIEscape.h"
#include "OnlineIdentityInterfaceEscape.h"
#include "VoiceInterfaceImpl.h"
#include "HAL/RunnableThread.h"

FThreadSafeCounter FOnlineSubsystemEscape::TaskCounter;

IOnlineSessionPtr FOnlineSubsystemEscape::GetSessionInterface() const
{
	return SessionInterface;
}

IOnlineFriendsPtr FOnlineSubsystemEscape::GetFriendsInterface() const
{
	return nullptr;
}

IOnlinePartyPtr FOnlineSubsystemEscape::GetPartyInterface() const
{
	return nullptr;
}

IOnlineGroupsPtr FOnlineSubsystemEscape::GetGroupsInterface() const
{
	return nullptr;
}

IOnlineSharedCloudPtr FOnlineSubsystemEscape::GetSharedCloudInterface() const
{
	return nullptr;
}

IOnlineUserCloudPtr FOnlineSubsystemEscape::GetUserCloudInterface() const
{
	return nullptr;
}

IOnlineLeaderboardsPtr FOnlineSubsystemEscape::GetLeaderboardsInterface() const
{
	return nullptr;
}

IOnlineVoicePtr FOnlineSubsystemEscape::GetVoiceInterface() const
{
	if (VoiceInterface.IsValid() && !bVoiceInterfaceInitialized)
	{
		if (!VoiceInterface->Init())
		{
			VoiceInterface = nullptr;
		}

		bVoiceInterfaceInitialized = true;
	}

	return VoiceInterface;
}

IOnlineExternalUIPtr FOnlineSubsystemEscape::GetExternalUIInterface() const
{
	return ExternalUIInterface;
}

IOnlineTimePtr FOnlineSubsystemEscape::GetTimeInterface() const
{
	return nullptr;
}

IOnlineIdentityPtr FOnlineSubsystemEscape::GetIdentityInterface() const
{
	return IdentityInterface;
}

IOnlineTitleFilePtr FOnlineSubsystemEscape::GetTitleFileInterface() const
{
	return nullptr;
}

IOnlineEntitlementsPtr FOnlineSubsystemEscape::GetEntitlementsInterface() const
{
	return nullptr;
}

IOnlineStorePtr FOnlineSubsystemEscape::GetStoreInterface() const
{
	return nullptr;
}

IOnlineStoreV2Ptr FOnlineSubsystemEscape::GetStoreV2Interface() const
{
	return nullptr;
}

IOnlinePurchasePtr FOnlineSubsystemEscape::GetPurchaseInterface() const
{
	return nullptr;
}

IOnlineEventsPtr FOnlineSubsystemEscape::GetEventsInterface() const
{
	return nullptr;
}

IOnlineAchievementsPtr FOnlineSubsystemEscape::GetAchievementsInterface() const
{
	return nullptr;
}

IOnlineSharingPtr FOnlineSubsystemEscape::GetSharingInterface() const
{
	return nullptr;
}

IOnlineUserPtr FOnlineSubsystemEscape::GetUserInterface() const
{
	return nullptr;
}

IOnlineMessagePtr FOnlineSubsystemEscape::GetMessageInterface() const
{
	return nullptr;
}

IOnlinePresencePtr FOnlineSubsystemEscape::GetPresenceInterface() const
{
	return nullptr;
}

IOnlineChatPtr FOnlineSubsystemEscape::GetChatInterface() const
{
	return nullptr;
}

IOnlineTurnBasedPtr FOnlineSubsystemEscape::GetTurnBasedInterface() const
{
	return nullptr;
}

bool FOnlineSubsystemEscape::IsLocalPlayer(const FUniqueNetId& UniqueId) const
{
	return false;
}

bool FOnlineSubsystemEscape::Init()
{
	OnlineAsyncTaskThreadRunnable = new FOnlineAsyncTaskManagerEscape(this);
	check(OnlineAsyncTaskThreadRunnable);
	OnlineAsyncTaskThread = FRunnableThread::Create(OnlineAsyncTaskThreadRunnable, *FString::Printf(TEXT("OnlineAsyncTaskThreadEscape %s(%d)"), *InstanceName.ToString(), TaskCounter.Increment()), 128 * 1024, TPri_Normal);
	check(OnlineAsyncTaskThread);

	UE_LOG_ONLINE(Verbose, TEXT("Created thread (ID:%d)."), OnlineAsyncTaskThread->GetThreadID());

	SessionInterface = MakeShareable(new FOnlineSessionEscape(this));
	IdentityInterface = MakeShareable(new FOnlineIdentityEscape(this));
	ExternalUIInterface = MakeShareable(new FOnlineExternalUIEscape(this));
	VoiceInterface = MakeShareable(new FOnlineVoiceImpl(this));

	return true;
}

bool FOnlineSubsystemEscape::Shutdown()
{
	UE_LOG_ONLINE(VeryVerbose, TEXT("FOnlineSubsystemEscape::Shutdown()"));

	FOnlineSubsystemImpl::Shutdown();

	if (OnlineAsyncTaskThread)
	{
		// Destroy the online async task thread
		delete OnlineAsyncTaskThread;
		OnlineAsyncTaskThread = nullptr;
	}

	if (OnlineAsyncTaskThreadRunnable)
	{
		delete OnlineAsyncTaskThreadRunnable;
		OnlineAsyncTaskThreadRunnable = nullptr;
	}

	if (VoiceInterface.IsValid() && bVoiceInterfaceInitialized)
	{
		VoiceInterface->Shutdown();
	}

#define DESTRUCT_INTERFACE(Interface) \
 	if (Interface.IsValid()) \
 	{ \
 		ensure(Interface.IsUnique()); \
 		Interface = nullptr; \
 	}

	// Destruct the interfaces
	DESTRUCT_INTERFACE(SessionInterface);
	DESTRUCT_INTERFACE(IdentityInterface);
	DESTRUCT_INTERFACE(ExternalUIInterface);
	DESTRUCT_INTERFACE(VoiceInterface);
	//DESTRUCT_INTERFACE(LeaderboardsInterface);

#undef DESTRUCT_INTERFACE

	return true;
}

bool FOnlineSubsystemEscape::Exec(class UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	return false;
}

bool FOnlineSubsystemEscape::IsEnabled() const
{
	return true;
}

FString FOnlineSubsystemEscape::GetAppId() const
{
	return TEXT("");
}

FText FOnlineSubsystemEscape::GetOnlineServiceName() const
{
	return FText::GetEmpty();
}

bool FOnlineSubsystemEscape::Tick(float DeltaTime)
{
	if (!FOnlineSubsystemImpl::Tick(DeltaTime))
	{
		return false;
	}

	if (OnlineAsyncTaskThreadRunnable)
	{
		OnlineAsyncTaskThreadRunnable->GameTick();
	}

	if (SessionInterface.IsValid())
	{
		SessionInterface->Tick(DeltaTime);
	}

	if (VoiceInterface.IsValid() && bVoiceInterfaceInitialized)
	{
		VoiceInterface->Tick(DeltaTime);
	}

	return true;
}

#if ESCAPE_BUILD_LOGINSERVER
void FOnlineSubsystemEscape::RunLogic()
{
	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	check(SocketSubsystem);

	check(Socket);
	FSocket* ClientSocket = Socket->Accept(ESCAPE_SUBSYSTEM);
	if (ClientSocket != nullptr)
	{
		ClientsSocket.Add(ClientSocket);

		TSharedPtr<FInternetAddr> InternetAddr = SocketSubsystem->CreateInternetAddr();

		ClientSocket->GetAddress(*InternetAddr);
		
		UE_LOG(LogNet, Log, TEXT("Accept : socket addr[%s]"), *(InternetAddr->ToString(true)));
	}

	FTimespan WaitTime(2000);

	for (FSocket* ClientSocket : ClientsSocket)
	{
		int32 BytesRead = 0;
		uint8 Data[MAX_PACKET_SIZE];
		uint8* DataRef = Data;

		if (!ClientSocket->Wait(ESocketWaitConditions::WaitForRead, WaitTime))
		{
			continue;
		}

		while (ClientSocket != nullptr)
		{
			bool bOk = false;
			{
				bOk = ClientSocket->Recv(Data, sizeof(Data), BytesRead);
			}

			if (bOk)
			{
				if (BytesRead == 0)
				{
					int32* DataSize = (int32*)DataRef;

					FUserLogin* UserLogin = (FUserLogin*)(DataSize + 1);

					UE_LOG(LogNet, Log, TEXT("Recv : UserName[%s] PassWord[%s]"), ANSI_TO_TCHAR(UserLogin->UserName), ANSI_TO_TCHAR(UserLogin->PassWord));

					//UE_LOG(LogNet, Log, TEXT("Recv : Data[%s]"), *FString((char*)DataRef));
					break;
				}
			}
			else
			{
				ClientsSocket.Remove(ClientSocket);

				TSharedPtr<FInternetAddr> InternetAddr = SocketSubsystem->CreateInternetAddr();

				ClientSocket->GetAddress(*InternetAddr);

				ESocketErrors Error = SocketSubsystem->GetLastErrorCode();

				UE_LOG(LogNet, Log, TEXT("SocketError : addr[%s] error[%d]"), *(InternetAddr->ToString(true)), (int32)Error);
				
				break;
			}
		}
	}
}
#endif

bool FOnlineSubsystemEscape::IsLogicServer()
{
	return bIsLogicServer;
}

void FOnlineSubsystemEscape::SetLogicServer(bool bLogicServer)
{
	bIsLogicServer = bLogicServer;
}