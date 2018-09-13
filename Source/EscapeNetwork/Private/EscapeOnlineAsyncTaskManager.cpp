// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "EscapeOnlineAsyncTaskManager.h"
#include "EscapeNetworkBase.h"


void FEscapeOnlineAsyncTaskManager::OnlineTick()
{
	check(EscapeNetwork);
	check(FPlatformTLS::GetCurrentThreadId() == OnlineThreadId);

	EscapeNetwork->Process();
}