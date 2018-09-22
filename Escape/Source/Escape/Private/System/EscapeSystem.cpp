// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "EscapeSystem.h"


UEscapeSystem::UEscapeSystem(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{

}

void UEscapeSystem::Register(UEscapeClient* EscapeClient)
{
	if (EscapeClient != nullptr)
	{
		EscapeUser = MakeShareable(new FEscapeUser());

		EscapeClient->AddMessageCallback(ELogicCode::USER_LOGIN, this, &UEscapeSystem::UpdateEscapeUser);
	}
}

TSharedPtr<FEscapeUser> UEscapeSystem::GetEscapeUser() const
{
	return EscapeUser;
}

void UEscapeSystem::UpdateEscapeUser(void* Data, EErrorCode Error)
{
	EscapeUser->Copy(Data);
}