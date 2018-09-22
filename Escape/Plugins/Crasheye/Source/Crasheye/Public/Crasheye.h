// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModuleManager.h"

class FCrasheyeModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	virtual void InitCrasheye(FString AppID);
	virtual void SetUserNameInfo(FString UserName);
	virtual void SetVersionInfo(FString Version);
	virtual void SetLeaveBreadcrumbInfo(FString Breadcrumb);
private: 
	bool bEnableCrasheye;
	FString AppID;
	FString BranchInfo;
};