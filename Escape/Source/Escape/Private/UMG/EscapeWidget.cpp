// Copyright 2018 by January. All Rights Reserved.

#include "EscapeWidget.h"
#include "EscapeSystem.h"
#include "EscapeGameInstance.h"
#include "EscapePlayerController.h"


UEscapeWidget::UEscapeWidget(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{

}

void UEscapeWidget::NativeConstruct()
{
	UWorld* World = GetWorld();
	UEscapeGameInstance* GameInstance = World ? Cast<UEscapeGameInstance>(World->GetGameInstance()) : nullptr;
	if (GameInstance != nullptr)
	{
		EscapeClient = GameInstance->GetEscapeClient();
		EscapeSystem = GameInstance->GetEscapeSystem();

		RegisterMessageCallback();
	}

	Super::NativeConstruct();
}

void UEscapeWidget::BeginDestroy()
{
	Super::BeginDestroy();

	if (EscapeClient != nullptr)
	{
		EscapeClient->ClearMessageCallback(this);
	}
}

class UEscapeClient* UEscapeWidget::GetEscapeClient() const
{
	return EscapeClient;
}

class UEscapeSystem* UEscapeWidget::GetEscapeSystem() const
{
	return EscapeSystem;
}

void UEscapeWidget::Reconnect()
{
	if (EscapeClient != nullptr)
	{
		EscapeClient->Reconnect();
	}
}

bool UEscapeWidget::IsConnected() const
{
	if (EscapeClient != nullptr)
	{
		return EscapeClient->IsConnected();
	}

	return false;
}

void UEscapeWidget::ReturnLobby()
{
	AEscapePlayerController* PlayerController = Cast<AEscapePlayerController>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->ReturnLobby();
	}
}

bool UEscapeWidget::GetEscapeUser(FEscapeUser& OutEscapeUser) const
{
	TSharedPtr<FEscapeUser> EscapeUser = EscapeSystem ? EscapeSystem->GetEscapeUser() : nullptr;
	if (EscapeUser.IsValid())
	{
		OutEscapeUser = *EscapeUser;

		return true;
	}

	return false;
}

void UEscapeWidget::RegisterMessageCallback()
{
	if (EscapeClient != nullptr)
	{
		EscapeClient->AddMessageCallback(LC_CONNECTION, this, &UEscapeWidget::NotifyConnection);
	}
}

void UEscapeWidget::NotifyConnection(void* Data, EErrorCode Error)
{
	ReceiveNotifyConnection((ENetworkConnection)Error);
}