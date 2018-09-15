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

void UEscapeMessageContrl::NotifyUserLogin(FSocket* Socket, void* Data, EErrorCode Error)
{
	FUserLogin* UserLogin = (FUserLogin*)Data;
	if (UserLogin != nullptr)
	{
		if (FPlatformString::Strncmp(ANSI_TO_TCHAR(UserLogin->Username), TEXT("909185693"), sizeof(UserLogin->Username)) == 0 &&
			FPlatformString::Strncmp(ANSI_TO_TCHAR(UserLogin->Password), TEXT("111111"), sizeof(UserLogin->Password)) == 0)
		{
			FEscapeUser SendData;
			SendData.ID = 10086;
			FPlatformString::Strncpy(SendData.Nickname, "Ø¼¼úÉÙ"), sizeof(SendData.Nickname));
			EscapeServer->SendTo(Socket, ELogicCode::USER_LOGIN, EErrorCode::NONE, sizeof(FEscapeUser), &Data);
		}
		else
		{
			EscapeServer->SendTo(Socket, ELogicCode::USER_LOGIN, EErrorCode::PASSWORD_ERROR);
		}
	}
}

void UEscapeMessageContrl::NotifyMatchGame(FSocket* Socket, void* Data, EErrorCode Error)
{

}

void UEscapeMessageContrl::NotifyInvitation(FSocket* Socket, void* Data, EErrorCode Error)
{

}