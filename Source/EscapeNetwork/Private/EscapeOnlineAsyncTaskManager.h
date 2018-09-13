// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "OnlineAsyncTaskManager.h"

class UEscapeNetworkBase;


class FEscapeOnlineAsyncTaskManager : public FOnlineAsyncTaskManager
{

public:
	FEscapeOnlineAsyncTaskManager(UEscapeNetworkBase* InEscapeNetwork) : EscapeNetwork(InEscapeNetwork)
	{

	}

protected:
	/** Cached reference to the main online subsystem */
	class UEscapeNetworkBase* EscapeNetwork;

	// FOnlineAsyncTaskManager
	virtual void OnlineTick() override;
};