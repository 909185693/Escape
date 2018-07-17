// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "OnlineIdentityInterfaceEscape.h"


TSharedPtr<FUserOnlineAccount> FOnlineIdentityEscape::GetUserAccount(const FUniqueNetId& UserId) const
{
	//@todo - not implemented
	return NULL;
}
TArray<TSharedPtr<FUserOnlineAccount> > FOnlineIdentityEscape::GetAllUserAccounts() const
{
	//@todo - not implemented
	return TArray<TSharedPtr<FUserOnlineAccount> >();
}

bool FOnlineIdentityEscape::Login(int32 LocalUserNum, const FOnlineAccountCredentials& AccountCredentials)
{
	return false;
}

bool FOnlineIdentityEscape::Logout(int32 LocalUserNum)
{
	return false;
}

bool FOnlineIdentityEscape::AutoLogin(int32 LocalUserNum)
{
	// Autologin for dedicated servers happens via session creation in the GameServerAPI LogOnAnonymous()
	return false;
}

ELoginStatus::Type FOnlineIdentityEscape::GetLoginStatus(int32 LocalUserNum) const
{
	return ELoginStatus::NotLoggedIn;
}

ELoginStatus::Type FOnlineIdentityEscape::GetLoginStatus(const FUniqueNetId& UserId) const
{
	return GetLoginStatus(0);
}

TSharedPtr<const FUniqueNetId> FOnlineIdentityEscape::GetUniquePlayerId(int32 LocalUserNum) const
{
	return NULL;
}

TSharedPtr<const FUniqueNetId> FOnlineIdentityEscape::CreateUniquePlayerId(uint8* Bytes, int32 Size)
{
	return NULL;
}

TSharedPtr<const FUniqueNetId> FOnlineIdentityEscape::CreateUniquePlayerId(const FString& Str)
{
	return nullptr;
}

/**
* Reads the player's nick name from the online service
*
* @param LocalUserNum the controller number of the associated user
*
* @return a string containing the players nick name
*/
FString FOnlineIdentityEscape::GetPlayerNickname(int32 LocalUserNum) const
{
	return FString(TEXT(""));
}

FString FOnlineIdentityEscape::GetPlayerNickname(const FUniqueNetId& UserId) const
{
	return FString(TEXT(""));
}

/**
* Gets a user's platform specific authentication token to verify their identity
*
* @param LocalUserNum the controller number of the associated user
*
* @return String representing the authentication token
*/
FString FOnlineIdentityEscape::GetAuthToken(int32 LocalUserNum) const
{
	FString ResultToken;
	return ResultToken;
}

void FOnlineIdentityEscape::RevokeAuthToken(const FUniqueNetId& UserId, const FOnRevokeAuthTokenCompleteDelegate& Delegate)
{
	//UE_LOG(LogOnline, Display, TEXT("FOnlineIdentityEscape::RevokeAuthToken not implemented"));
}

void FOnlineIdentityEscape::GetUserPrivilege(const FUniqueNetId& UserId, EUserPrivileges::Type Privilege, const FOnGetUserPrivilegeCompleteDelegate& Delegate)
{
	Delegate.ExecuteIfBound(UserId, Privilege, (uint32)EPrivilegeResults::NoFailures);
}

FPlatformUserId FOnlineIdentityEscape::GetPlatformUserIdFromUniqueNetId(const FUniqueNetId& UniqueNetId) const
{
	return PLATFORMUSERID_NONE;
}

FString FOnlineIdentityEscape::GetAuthType() const
{
	return TEXT("");
}