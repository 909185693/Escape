// Copyright 2018 by January. All Rights Reserved.

#include "EscapeWidget.h"
#include "EscapeGameInstance.h"


UEscapeWidget::UEscapeWidget(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
}

bool UEscapeWidget::Initialize()
{
	Super::Initialize();

	UWorld* World = GetWorld();
	UEscapeGameInstance* GameInstance = World ? Cast<UEscapeGameInstance>(World->GetGameInstance()) : nullptr;
	if (GameInstance != nullptr)
	{
		EscapeClient = GameInstance->GetEscapeClient();

		RegisterMessageCallback();
	}

	return true;
}

void UEscapeWidget::BeginDestroy()
{
	Super::BeginDestroy();

	if (EscapeClient != nullptr)
	{
		EscapeClient->ClearMessageCallback(this);
	}
}

UEscapeClient* UEscapeWidget::GetEscapeClient() const
{
	return EscapeClient;
}

void UEscapeWidget::Reconnect()
{
	if (EscapeClient != nullptr)
	{
		EscapeClient->Reconnect();
	}
}

void UEscapeWidget::RegisterMessageCallback()
{
	if (EscapeClient != nullptr)
	{
		EscapeClient->AddMessageCallback(ELogicCode::CONNECTION, this, &UEscapeWidget::NotifyConnection);
	}
}

void UEscapeWidget::NotifyConnection(void* Data, EErrorCode Error)
{
	ReceiveNotifyConnection((ENetworkConnection)Error);
}