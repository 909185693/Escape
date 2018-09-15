// Copyright 2018 by January. All Rights Reserved.

#include "EscapeLobbyWidget.h"


UEscapeLobbyWidget::UEscapeLobbyWidget(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{

}

void UEscapeLobbyWidget::UserLogin(const FText& Username, const FText& Password)
{
	if (EscapeClient)
	{
		FUserLogin Data;

		FPlatformString::Strcpy(Data.Username, sizeof(Data.Username), TCHAR_TO_ANSI(*Username.ToString()));
		FPlatformString::Strcpy(Data.Password, sizeof(Data.Password), TCHAR_TO_ANSI(*Password.ToString()));

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
		NotifyUserLoginPtr = EscapeClient->AddMessageCallback(ELogicCode::USER_LOGIN, this, &UEscapeLobbyWidget::NotifyUserLogin);
		NotifyUserLoginPtr = EscapeClient->AddMessageCallback(ELogicCode::USER_LOGIN, this, &UEscapeLobbyWidget::NotifyUserLogin);
		NotifyUserLoginPtr = EscapeClient->AddMessageCallback(ELogicCode::USER_LOGIN, this, &UEscapeLobbyWidget::NotifyUserLogin);
	}
}

void UEscapeLobbyWidget::UnregisterMessageCallback()
{
	Super::UnregisterMessageCallback();

	if (EscapeClient != nullptr)
	{
		EscapeClient->RemoveMessageCallback(NotifyUserLoginPtr);
		EscapeClient->RemoveMessageCallback(NotifyMatchGamePtr);
		EscapeClient->RemoveMessageCallback(NotifyInvitationPtr);
	}
}

void UEscapeLobbyWidget::NotifyUserLogin(void* Data, EErrorCode Error)
{
	ReceiveNotifyUserLogin(Error == EErrorCode::NONE);
}

void UEscapeLobbyWidget::NotifyMatchGame(void* Data, EErrorCode Error)
{
	ReceiveNotifyMatchGame(Error == EErrorCode::NONE);
}

void UEscapeLobbyWidget::NotifyInvitation(void* Data, EErrorCode Error)
{
	ReceiveNotifyInvitation(Error == EErrorCode::NONE);
}
