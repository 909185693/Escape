// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "CrasheyePrivatePCH.h"

#define LOCTEXT_NAMESPACE "FCrasheyeModule"

void FCrasheyeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FCrasheyeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

void FCrasheyeModule::InitCrasheye(FString AppID)
{
	if (!bEnableCrasheye)
	{
		return;
	}

}

void FCrasheyeModule::SetUserNameInfo(FString UserName)
{
	if (!bEnableCrasheye)
	{
		return;
	}

}

void FCrasheyeModule::SetVersionInfo(FString Version)
{
	if (!bEnableCrasheye)
	{
		return;
	}

}

void FCrasheyeModule::SetLeaveBreadcrumbInfo(FString Breadcrumb)
{
	if (!bEnableCrasheye)
	{
		return;
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCrasheyeModule, Crasheye)