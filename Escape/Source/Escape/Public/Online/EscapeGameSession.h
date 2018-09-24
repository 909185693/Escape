// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "Online.h"
#include "GameFramework/GameSession.h"
#include "Escape.h"
#include "EscapeNetwork.h"
#include "EscapeGameSession.generated.h"


UCLASS()
class ESCAPE_API AEscapeGameSession : public AGameSession
{
	GENERATED_UCLASS_BODY()
	
	virtual void Destroyed() override;

	/** Initialize options based on passed in options string */
	virtual void InitOptions(const FString& Options) override;

	/**
	* Called when this instance is starting up as a dedicated server
	*/
	virtual void RegisterServer() override;

	/// UEscapeGameInstance
	class UEscapeClient* GetEscapeClient() const;

#if ESCAPE_BUILD_SERVER
	virtual void NotifyConnection(void* Data, EErrorCode Error);
	virtual void NotifyRegisterServer(void* Data, EErrorCode Error);
#endif

protected:
	FString Guid;
	
	UPROPERTY(Transient)
	UEscapeClient* EscapeClient;

	UPROPERTY(Config)
	FString EscapeClientClassName;

	UPROPERTY(Config)
	TSubclassOf<UEscapeClient> EscapeClientClass;
};
