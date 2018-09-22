// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine.h"
#include "CrasheyeHelper.generated.h"


/**
 * 
 */
UCLASS()
class CRASHEYE_API UCrasheyeHelper : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "Crasheye")
	static void CrasheyeSetUserIdentifier(FString UserIdentifier);
	
	UFUNCTION(BlueprintCallable, Category = "Crasheye")
	static void CrasheyeSetAppVersion(FString Version);

	UFUNCTION(BlueprintCallable, Category = "Crasheye")
	static void CrasheyeLeaveBreadcrumb(FString Breadcrumb);


	UFUNCTION(BlueprintCallable, Category = "Crasheye")
	static void CrasheyeTestCrash();
};
