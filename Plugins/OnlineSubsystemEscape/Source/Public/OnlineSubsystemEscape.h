// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemImpl.h"
#include "OnlineSubsystemEscapePackage.h"

class FOnlineSessionEscape;
class FOnlineIdentityEscape;
class FOnlineExternalUIEscape;
class FOnlineVoiceImpl;

typedef TSharedPtr<class FOnlineSessionEscape, ESPMode::ThreadSafe> FOnlineSessionEscapePtr;
typedef TSharedPtr<class FOnlineIdentityEscape, ESPMode::ThreadSafe> FOnlineIdentityEscapePtr;
typedef TSharedPtr<class FOnlineExternalUIEscape, ESPMode::ThreadSafe> FOnlineExternalUIEscapePtr;
typedef TSharedPtr<class FOnlineVoiceImpl, ESPMode::ThreadSafe> FOnlineVoiceImplPtr;

class ONLINESUBSYSTEMESCAPE_API FOnlineSubsystemEscape :
	public FOnlineSubsystemImpl
{
protected:
	/** Interface to online session */
	FOnlineSessionEscapePtr SessionInterface;

	/** Interface to the profile services */
	FOnlineIdentityEscapePtr IdentityInterface;
	
	/** Interface to implementation of the external ui */
	FOnlineExternalUIEscapePtr ExternalUIInterface;

	/** Interface for voice communication */
	mutable FOnlineVoiceImplPtr VoiceInterface;
	
	/** Interface for voice communication */
	mutable bool bVoiceInterfaceInitialized;

	/** Online async task runnable */
	class FOnlineAsyncTaskManagerEscape* OnlineAsyncTaskThreadRunnable;

	/** Online async task thread */
	class FRunnableThread* OnlineAsyncTaskThread;

	// task counter, used to generate unique thread names for each task
	static FThreadSafeCounter TaskCounter;

PACKAGE_SCOPE:

	/** Only the factory makes instances */
	FOnlineSubsystemEscape() :
		bVoiceInterfaceInitialized(false),
		OnlineAsyncTaskThreadRunnable(nullptr),
		OnlineAsyncTaskThread(nullptr),
		SessionInterface(nullptr),
		IdentityInterface(nullptr),
		ExternalUIInterface(nullptr),
		VoiceInterface(nullptr)
	{

	}

	FOnlineSubsystemEscape(FName InInstanceName) :
		FOnlineSubsystemImpl(ESCAPE_SUBSYSTEM, InInstanceName),
		bVoiceInterfaceInitialized(false),
		OnlineAsyncTaskThreadRunnable(nullptr),
		OnlineAsyncTaskThread(nullptr),
		SessionInterface(nullptr),
		IdentityInterface(nullptr),
		ExternalUIInterface(nullptr),
		VoiceInterface(nullptr)
	{

	}

public:

	virtual ~FOnlineSubsystemEscape()
	{
	}

	// IOnlineSubsystem

	virtual IOnlineSessionPtr GetSessionInterface() const override;
	virtual IOnlineFriendsPtr GetFriendsInterface() const override;
	virtual IOnlinePartyPtr GetPartyInterface() const override;
	virtual IOnlineGroupsPtr GetGroupsInterface() const override;
	virtual IOnlineSharedCloudPtr GetSharedCloudInterface() const override;
	virtual IOnlineUserCloudPtr GetUserCloudInterface() const override;
	virtual IOnlineLeaderboardsPtr GetLeaderboardsInterface() const override;
	virtual IOnlineVoicePtr GetVoiceInterface() const override;
	virtual IOnlineExternalUIPtr GetExternalUIInterface() const override;
	virtual IOnlineTimePtr GetTimeInterface() const override;
	virtual IOnlineIdentityPtr GetIdentityInterface() const override;
	virtual IOnlineTitleFilePtr GetTitleFileInterface() const override;
	virtual IOnlineEntitlementsPtr GetEntitlementsInterface() const override;
	virtual IOnlineStorePtr GetStoreInterface() const override;
	virtual IOnlineStoreV2Ptr GetStoreV2Interface() const override;
	virtual IOnlinePurchasePtr GetPurchaseInterface() const override;
	virtual IOnlineEventsPtr GetEventsInterface() const override;
	virtual IOnlineAchievementsPtr GetAchievementsInterface() const override;
	virtual IOnlineSharingPtr GetSharingInterface() const override;
	virtual IOnlineUserPtr GetUserInterface() const override;
	virtual IOnlineMessagePtr GetMessageInterface() const override;
	virtual IOnlinePresencePtr GetPresenceInterface() const override;
	virtual IOnlineChatPtr GetChatInterface() const override;
	virtual IOnlineTurnBasedPtr GetTurnBasedInterface() const override;
	virtual bool IsLocalPlayer(const FUniqueNetId& UniqueId) const override;
	virtual bool Init() override;
	virtual bool Shutdown() override;
	virtual bool Exec(class UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;
	virtual bool IsEnabled() const override;
	virtual FString GetAppId() const override;
	virtual FText GetOnlineServiceName() const override;

	// FTickerObjectBase

	virtual bool Tick(float DeltaTime) override;

	// FOnlineSubsystemNull
};

typedef TSharedPtr<FOnlineSubsystemEscape, ESPMode::ThreadSafe> FOnlineSubsystemEscapePtr;