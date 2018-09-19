// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EscapeNetwork.h"
#include "Blueprint/UserWidget.h"
#include "EscapeWidget.generated.h"


UENUM(BlueprintType)
enum ENetworkConnection
{
	Network_Success,
	Network_Error = EErrorCode::NETWORK_ERROR,
	Network_Invalid = EErrorCode::INVALID_DATA
};

UCLASS()
class ESCAPE_API UEscapeWidget : public UUserWidget
{
	GENERATED_UCLASS_BODY()
	
	virtual bool Initialize() override;
	virtual void BeginDestroy() override;

	/// UEscapeWidget
	virtual UEscapeClient* GetEscapeClient() const;

protected:
	UFUNCTION(BlueprintCallable, Category = "Network")
	void Reconnect();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void ReturnLobby();

protected:
	virtual void RegisterMessageCallback();

	virtual void NotifyConnection(void* Data, EErrorCode Error);
	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "NotifyConnection"))
	void ReceiveNotifyConnection(ENetworkConnection NetworkConnection);

protected:
	UPROPERTY(Transient)
	UEscapeClient* EscapeClient;
};
