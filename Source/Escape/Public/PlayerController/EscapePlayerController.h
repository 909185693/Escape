// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "EscapeNetwork.h"
#include "EscapePlayerController.generated.h"


UCLASS()
class ESCAPE_API AEscapePlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()
	
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	/// UEscapeGameInstance
	class UEscapeClient* GetEscapeClient() const;

	virtual void NotifyClientTravel(void* Data, EErrorCode Error);

protected:
	UPROPERTY()
	UEscapeClient* EscapeClient;

	UPROPERTY(Transient)
	UUserWidget* UserWidget;

	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<UUserWidget> UserWidgetClass;
	
};
