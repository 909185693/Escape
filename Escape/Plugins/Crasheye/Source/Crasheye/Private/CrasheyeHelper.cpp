// Fill out your copyright notice in the Description page of Project Settings.
#include "CrasheyePrivatePCH.h"
#include "CrasheyeHelper.h"
#include "Crasheye.h"


UCrasheyeHelper::UCrasheyeHelper(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UCrasheyeHelper::CrasheyeSetUserIdentifier(FString UserIdentifier)
{
	FCrasheyeModule* CrasheyeModule = FModuleManager::GetModulePtr<FCrasheyeModule>("Crasheye");
	if(CrasheyeModule)
	{
		CrasheyeModule->SetUserNameInfo(UserIdentifier);
	}
}


void UCrasheyeHelper::CrasheyeSetAppVersion(FString Version)
{
	FCrasheyeModule* CrasheyeModule = FModuleManager::GetModulePtr<FCrasheyeModule>("Crasheye");
	if (CrasheyeModule)
	{
		CrasheyeModule->SetVersionInfo(Version);
	}
}


void UCrasheyeHelper::CrasheyeLeaveBreadcrumb(FString Breadcrumb)
{
	FCrasheyeModule* CrasheyeModule = FModuleManager::GetModulePtr<FCrasheyeModule>("Crasheye");
	if (CrasheyeModule)
	{
		CrasheyeModule->SetLeaveBreadcrumbInfo(Breadcrumb);
	}
}


void UCrasheyeHelper::CrasheyeTestCrash()
{
	int *ptr = NULL;
	(*ptr) = 1; // trigger crash
}

