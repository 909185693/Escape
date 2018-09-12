// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineAsyncTaskManager.h"
#include "OnlineSubsystemEscapePackage.h"


class FOnlineAsyncTaskManagerEscape :
	public FOnlineAsyncTaskManager
{
protected:

	/** Cached reference to the main online subsystem */
	class FOnlineSubsystemEscape* EscapeSubsystem;

public:

	FOnlineAsyncTaskManagerEscape(class FOnlineSubsystemEscape* InOnlineSubsystem)
		: EscapeSubsystem(InOnlineSubsystem)
	{
	}

	~FOnlineAsyncTaskManagerEscape()
	{
	}

	// FOnlineAsyncTaskManager
	virtual void OnlineTick() override;
};