// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "OnlineSubsystemEscape.h"

IOnlineSessionPtr FOnlineSubsystemEscape::GetSessionInterface() const
{
	return nullptr;
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
	return nullptr;
}

IOnlineExternalUIPtr FOnlineSubsystemEscape::GetExternalUIInterface() const
{
	return nullptr;
}

IOnlineTimePtr FOnlineSubsystemEscape::GetTimeInterface() const
{
	return nullptr;
}

IOnlineIdentityPtr FOnlineSubsystemEscape::GetIdentityInterface() const
{
	return nullptr;
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
	return false;
}

bool FOnlineSubsystemEscape::Shutdown()
{
	return false;
}

bool FOnlineSubsystemEscape::Exec(class UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	return false;
}

bool FOnlineSubsystemEscape::IsEnabled() const
{
	return false;
}

FString FOnlineSubsystemEscape::GetAppId() const
{
	return TEXT("");
}

FText FOnlineSubsystemEscape::GetOnlineServiceName() const
{
	return FText::GetEmpty();
}
