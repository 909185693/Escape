// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "CrasheyePrivatePCH.h"
#include "ConfigCacheIni.h"



#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#endif


#define LOCTEXT_NAMESPACE "FCrasheyeModule"


void FCrasheyeModule::StartupModule()
{
	
	check(GConfig);
	bool bLoad = GConfig->GetBool(TEXT("/Script/Crasheye.CrasheyeRuntimeSettings"), TEXT("Andoird_bEnableCrasheye"), bEnableCrasheye, GEngineIni);
	bLoad &= GConfig->GetString(TEXT("/Script/Crasheye.CrasheyeRuntimeSettings"), TEXT("Andoird_AppKey"), AppID, GEngineIni);
	if (bLoad)
	{
		InitCrasheye(AppID);
		/*
		if (GConfig->GetString(TEXT("/Script/Crasheye.CrasheyeRuntimeSettings"), TEXT("BranchInfo"), BranchInfo, GEngineIni))
		{
			SetLeaveBreadcrumbInfo(BranchInfo);
		}
		*/
	
	}
	
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FCrasheyeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

void FCrasheyeModule::InitCrasheye(FString appID)
{
	if (!bEnableCrasheye)
	{
		return;
	}

	
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jstring jAppID = Env->NewStringUTF(TCHAR_TO_UTF8(*appID));

		jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_SetAppIDToCrasheye", "(Ljava/lang/String;)V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, jAppID);
		Env->DeleteLocalRef(jAppID);
	}
#endif

}

void FCrasheyeModule::SetUserNameInfo(FString UserName)
{
	if (!bEnableCrasheye)
	{
		return;
	}

	
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jstring jUserName = Env->NewStringUTF(TCHAR_TO_UTF8(*UserName));

		jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_SetUserNameToCrasheye", "(Ljava/lang/String;)V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, jUserName);
		Env->DeleteLocalRef(jUserName);
	}
#endif
}

void FCrasheyeModule::SetVersionInfo(FString Version)
{
	if (!bEnableCrasheye)
	{
		return;
	}

	 
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jstring jVersion = Env->NewStringUTF(TCHAR_TO_UTF8(*Version));
		jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_SetVersionToCrasheye", "(Ljava/lang/String;)V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, jVersion);
		Env->DeleteLocalRef(jVersion);
	}
#endif
 
}


void FCrasheyeModule::SetLeaveBreadcrumbInfo(FString Breadcrumb)
{
	if (!bEnableCrasheye)
	{
		return;
	}

	 
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jstring jBreadcrumb = Env->NewStringUTF(TCHAR_TO_UTF8(*Breadcrumb));
		jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_SetLeaveBreadcrumbInfo", "(Ljava/lang/String;)V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, jBreadcrumb);
		Env->DeleteLocalRef(jBreadcrumb);
	}
#endif

 
}




#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCrasheyeModule, Crasheye)