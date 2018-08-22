// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "CrasheyePrivatePCH.h"
#include "ConfigCacheIni.h"

#if PLATFORM_IOS
#include <Crasheye/Crasheye.h>
#endif

#define LOCTEXT_NAMESPACE "FCrasheyeModule"

void FCrasheyeModule::StartupModule()
{
	check(GConfig);
	bool bLoad = GConfig->GetBool(TEXT("/Script/Crasheye.CrasheyeRuntimeSettings"), TEXT("IOS_bEnableCrasheye"), bEnableCrasheye, GEngineIni);
	bLoad &= GConfig->GetString(TEXT("/Script/Crasheye.CrasheyeRuntimeSettings"), TEXT("IOS_AppKey"), AppID, GEngineIni);
	if (bLoad)
	{
		InitCrasheye(AppID);
		if (GConfig->GetString(TEXT("/Script/Crasheye.CrasheyeRuntimeSettings"), TEXT("BranchInfo"), BranchInfo, GEngineIni))
		{
			SetLeaveBreadcrumbInfo(BranchInfo);
		}
	}
	//[[IOSTapJoy GetDelegate] performSelectorOnMainThread:@selector(StartupTapJoy) withObject:nil waitUntilDone : NO];
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
	NSString *appID = [NSString stringWithFString : AppID];
	[Crasheye initWithAppKey: appID];
}

void FCrasheyeModule::SetUserNameInfo(FString UserName)
{
	if (!bEnableCrasheye)
	{
		return;
	}
	NSString *userName = [NSString stringWithFString : UserName];
	[Crasheye setUserID : userName];
}

void FCrasheyeModule::SetVersionInfo(FString Version)
{
	if (!bEnableCrasheye)
	{
		return;
	}
	NSString *version = [NSString stringWithFString : Version];
	[Crasheye setAppVersion:version];
}

void FCrasheyeModule::SetLeaveBreadcrumbInfo(FString Breadcrumb)
{
	if (!bEnableCrasheye)
	{
		return;
	}
	NSString *breadcrumb = [NSString stringWithFString : Breadcrumb];
	[Crasheye leaveBreadcrumb : breadcrumb];
}
#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCrasheyeModule, Crasheye)