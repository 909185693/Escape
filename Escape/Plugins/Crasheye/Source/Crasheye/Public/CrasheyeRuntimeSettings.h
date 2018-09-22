#pragma once


#include "CrasheyeRuntimeSettings.generated.h"

/**
* Implements the settings for the Crasheye plugin.
*/
UCLASS(config = Engine, defaultconfig)
class CRASHEYE_API UCrasheyeRuntimeSettings : public UObject
{
	GENERATED_UCLASS_BODY()

	// Enables experimental *incomplete and unsupported* texture atlas groups that sprites can be assigned to
	UPROPERTY(EditAnywhere, config, Category = "Crasheye|Android")
	bool Andoird_bEnableCrasheye;

	// Enables experimental *incomplete and unsupported* texture atlas groups that sprites can be assigned to
	UPROPERTY(EditAnywhere, config, Category = "Crasheye|Android")
	FString Andoird_AppKey;

	// Enables experimental *incomplete and unsupported* texture atlas groups that sprites can be assigned to
	UPROPERTY(EditAnywhere, config, Category = "Crasheye|IOS")
	bool IOS_bEnableCrasheye;

	// Enables experimental *incomplete and unsupported* texture atlas groups that sprites can be assigned to
	UPROPERTY(EditAnywhere, config, Category = "Crasheye|IOS")
	FString IOS_AppKey;

	//UPROPERTY(EditAnywhere, config, Category = "Crasheye|Common")
	//FString BranchInfo;

	//UPROPERTY(EditAnywhere, config, Category = "Crasheye|Common")
	//bool FlushOnlyOverWiFi;
};
