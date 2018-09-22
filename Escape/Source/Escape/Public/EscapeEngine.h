// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameEngine.h"
#include "EscapeEngine.generated.h"


class UEscapeGameInstance;

/**
 * 
 */
UCLASS(config=Engine, transient)
class ESCAPE_API UEscapeEngine : public UGameEngine
{
	GENERATED_BODY()
	
	/* Hook up specific callbacks */
	virtual void Init(IEngineLoop* InEngineLoop) override;
	virtual void Start() override;
	virtual void Tick(float DeltaSeconds, bool bIdleMode) override;
	
protected:
	/** Event to gather up all net drivers and call TickDispatch at once */
	DECLARE_EVENT_OneParam(UEscapeEngine, FOnEscapeEngineTickEvent, float)
	FOnEscapeEngineTickEvent TickDispatchEvent;

public:
	/** Get the event that broadcasts TickDispatch */
	FOnEscapeEngineTickEvent& OnTickDispatch() { return TickDispatchEvent; }
};
