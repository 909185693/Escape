// Copyright 2018 by January. All Rights Reserved.

#include "EscapePlayerController.h"


AEscapePlayerController::AEscapePlayerController(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{

}

void AEscapePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UserWidgetClass != nullptr)
	{
		UserWidget = CreateWidget(this, UserWidgetClass);
		UserWidget->AddToViewport();
	}
}

void AEscapePlayerController::Destroyed()
{
	Super::Destroyed();

	if (UserWidget != nullptr)
	{
		UserWidget->RemoveFromViewport();
		UserWidget = nullptr;
	}
}