// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "EscapeMessageContrl.h"
#include "EscapeServer.h"


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
	}
}

void UEscapeMessageContrl::BeginDestroy()
{
	Super::BeginDestroy();
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
			FPlatformString::Strncpy(ANSI_TO_TCHAR(SendData.Nickname), TEXT("Ø¼¼úÉÙ"), sizeof(SendData.Nickname));
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

}

void UEscapeMessageContrl::NotifyInvitation(FConnection& Connection, void* Data, EErrorCode Error)
{

}