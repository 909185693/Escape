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
		FMatchGame Data;

		EscapeClient->Send(ELogicCode::MATCH_GAME, sizeof(FMatchGame), &Data);
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
		EscapeClient->AddMessageCallback(ELogicCode::MATCH_GAME, this, &UEscapeLobbyWidget::NotifyMatchGame);
		EscapeClient->AddMessageCallback(ELogicCode::INVITATION, this, &UEscapeLobbyWidget::NotifyInvitation);
	}
}

void UEscapeLobbyWidget::NotifyUserLogin(void* Data, EErrorCode Error)
{
	FEscapeUser EscapeUser;

	EUserLogin NotifyCode = Login_Success;
	switch (Error)
	{
	case EErrorCode::NONE:
	{
		EscapeUser.ID = *((int32*)(Data));
		char* NickName = (char*)((char*)Data + sizeof(int32));
		EscapeUser.Nickname = UTF8_TO_TCHAR(NickName);
	}
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

	UE_LOG(LogEscape, Log, TEXT("NotifyUserLogin => ID[%d] Nickname[%s]"), EscapeUser.ID, *EscapeUser.Nickname);

	ReceiveNotifyUserLogin(NotifyCode, EscapeUser);
}

void UEscapeLobbyWidget::NotifyMatchGame(void* Data, EErrorCode Error)
{
	ReceiveNotifyMatchGame(Error == EErrorCode::NONE);
}

void UEscapeLobbyWidget::NotifyInvitation(void* Data, EErrorCode Error)
{
	ReceiveNotifyInvitation(Error == EErrorCode::NONE);
}
