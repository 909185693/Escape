// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "OnlineAsyncTaskManagerEscape.h"
#include "OnlineSubsystemEscapeTypes.h"
#include "OnlineSubsystemEscape.h"


void FOnlineAsyncTaskManagerEscape::OnlineTick()
{
	check(EscapeSubsystem);
	check(FPlatformTLS::GetCurrentThreadId() == OnlineThreadId || !FPlatformProcess::SupportsMultithreading());

#if ESCAPE_BUILD_LOGINSERVER
	if (EscapeSubsystem->IsLogicServer())
	{
		EscapeSubsystem->RunLogic();
	}
#endif
}