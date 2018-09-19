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

		EscapeClient->Send(ELogicCode::USER_LOGIN, sizeof(FUserLogin), &Data);
	}
}

void UEscapeLobbyWidget::MatchGame()
{
	if (EscapeClient)
	{
		EscapeClient->Send(ELogicCode::MATCH_GAME);
	}
}

void UEscapeLobbyWidget::CancelMatch()
{
	if (EscapeClient)
	{
		EscapeClient->Send(ELogicCode::CANCEL_MATCH);
	}
}

void UEscapeLobbyWidget::Invitation(const int32 UserID)
{
	if (EscapeClient)
	{
		FInvitation Data;
		Data.UserID = UserID;

		EscapeClient->Send(ELogicCode::INVALID, sizeof(FInvitation), &Data);
	}
}

void UEscapeLobbyWidget::RegisterMessageCallback()
{
	Super::RegisterMessageCallback();

	if (EscapeClient != nullptr)
	{
		EscapeClient->AddMessageCallback(ELogicCode::USER_LOGIN, this, &UEscapeLobbyWidget::NotifyUserLogin);
		EscapeClient->AddMessageCallback(ELogicCode::INVITATION, this, &UEscapeLobbyWidget::NotifyInvitation);
	}
}

void UEscapeLobbyWidget::NotifyUserLogin(void* Data, EErrorCode Error)
{
	EUserLogin NotifyCode = Login_Success;
	switch (Error)
	{
	case EErrorCode::NONE:
		break;
	case EErrorCode::NETWORK_ERROR:
		NotifyCode = Login_NetworkError;
		break;
	case EErrorCode::PASSWORD_ERROR:
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
	ReceiveNotifyInvitation(Error == EErrorCode::NONE);
}
