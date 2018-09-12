// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModuleManager.h"

class FOnlineSubsystemEscapeModule : public IModuleInterface
{
private:
	/** Class responsible for creating instance(s) of the subsystem */
	class FOnlineFactoryEscape* EscapeFactory;

public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};