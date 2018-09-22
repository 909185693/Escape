// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "EscapeNetworkModule.h"


#define LOCTEXT_NAMESPACE "FEscapeNetworkModule"

void FEscapeNetworkModuleModule::StartupModule()
{
	
}

void FEscapeNetworkModuleModule::ShutdownModule()
{
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEscapeNetworkModuleModule, EscapeNetwork)

DEFINE_LOG_CATEGORY(LogEscapeNetwork)