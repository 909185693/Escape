// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "CrasheyePrivatePCH.h"


#if UE_EDITOR
#include "ISettingsModule.h"
#include "CrasheyeRuntimeSettings.h"
#endif


#define LOCTEXT_NAMESPACE "FCrasheyeModule"

void FCrasheyeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	// register settings


 
#if UE_EDITOR
		ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

		if (SettingsModule != nullptr)
		{
			SettingsModule->RegisterSettings("Project", "Plugins", "Crasheye",
				LOCTEXT("RuntimeSettingsName", "Crasheye"),
				LOCTEXT("RuntimeSettingsDescription", "Project settings for Crasheye"),
				GetMutableDefault<UCrasheyeRuntimeSettings>()
				);
		}
#endif


}

void FCrasheyeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

/* LDS: πÿ±’±‡º≠∆˜…Ë÷√ */
#if UE_EDITOR
	{
		ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

		if (SettingsModule != nullptr)
		{
			SettingsModule->UnregisterSettings("Project", "Plugins", "Crasheye");
		}
	}
#endif

}
void FCrasheyeModule::InitCrasheye(FString appID)
{

}
void FCrasheyeModule::SetUserNameInfo(FString UserName)
{

}

void FCrasheyeModule::SetVersionInfo(FString Version)
{
}

void FCrasheyeModule::SetLeaveBreadcrumbInfo(FString Breadcrumb)
{
}
#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCrasheyeModule, Crasheye)