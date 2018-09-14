// Copyright 2018 by January. All Rights Reserved.

#include "EscapeUserLoginWidget.h"


UEscapeUserLoginWidget::UEscapeUserLoginWidget(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{

}

void UEscapeUserLoginWidget::UserLogin(const FText& Username, const FText& Password)
{
	if (EscapeClient)
	{
		FUserLogin UserLogin;

		FPlatformString::Strcpy(UserLogin.Username, sizeof(UserLogin.Username), TCHAR_TO_ANSI(*Username.ToString()));
		FPlatformString::Strcpy(UserLogin.Password, sizeof(UserLogin.Password), TCHAR_TO_ANSI(*Password.ToString()));

		EscapeClient->Send(ELogicCode::USER_LOGIN, sizeof(FUserLogin), &UserLogin);
	}
}

void UEscapeUserLoginWidget::BindEscapeClientEvent()
{
	Super::BindEscapeClientEvent();

	if (EscapeClient != nullptr)
	{
		EscapeClient->AddMessageCallback(ELogicCode::USER_LOGIN, this, &UEscapeUserLoginWidget::NotifyNotifyUserLogin);
	}
}

void UEscapeUserLoginWidget::NotifyNotifyUserLogin(void* Data, EErrorCode Error)
{
	ReceiveNotifyUserLogin((EUserLogin)Error);
}
