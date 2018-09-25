// Copyright 2018 by January. All Rights Reserved.

#include "EscapeLobbyWidget.h"
#include "Escape.h"


UEscapeLobbyWidget::UEscapeLobbyWidget(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{

}

void UEscapeLobbyWidget::UserLogin(const FText& Username, const FText& Password)
{
	if (EscapeClient)
	{
		FUserLogin Data;

		FPlatformString::Strncpy(Data.Username, TCHAR_TO_ANSI(*Username.ToString()), sizeof(Data.Username));
		FPlatformString::Strncpy(Data.Password, TCHAR_TO_ANSI(*Password.ToString()), sizeof(Data.Password));

		EscapeClient->Send(LC_USERLOGIN, sizeof(FUserLogin), &Data);
	}
}

void UEscapeLobbyWidget::MatchGame()
{
	if (EscapeClient)
	{
		EscapeClient->Send(LC_MATCHGAME);
	}
}

void UEscapeLobbyWidget::CancelMatch()
{
	if (EscapeClient)
	{
		EscapeClient->Send(LC_CANCELMATCH);
	}
}

void UEscapeLobbyWidget::Invitation(const int32 UserID)
{
	if (EscapeClient)
	{
		FInvitation Data;
		Data.UserID = UserID;

		EscapeClient->Send(LC_INVALID, sizeof(FInvitation), &Data);
	}
}

void UEscapeLobbyWidget::RegisterMessageCallback()
{
	Super::RegisterMessageCallback();

	if (EscapeClient != nullptr)
	{
		EscapeClient->AddMessageCallback(LC_USERLOGIN, this, &UEscapeLobbyWidget::NotifyUserLogin);
		EscapeClient->AddMessageCallback(LC_INVITATION, this, &UEscapeLobbyWidget::NotifyInvitation);
	}
}

void UEscapeLobbyWidget::NotifyUserLogin(void* Data, EErrorCode Error)
{
	EUserLogin NotifyCode = Login_Success;
	switch (Error)
	{
	case EC_NONE:
		break;
	case EC_NETWORKERROR:
		NotifyCode = Login_NetworkError;
		break;
	case EC_PASSWORDERROR:
		NotifyCode = Login_PasswordError;
		break;
	default:
		NotifyCode = Login_NoneError;
		break;
	}

	ReceiveNotifyUserLogin(NotifyCode);
}

void UEscapeLobbyWidget::NotifyInvitation(void* Data, EErrorCode Error)
{
	ReceiveNotifyInvitation(Error == EC_NONE);
}
