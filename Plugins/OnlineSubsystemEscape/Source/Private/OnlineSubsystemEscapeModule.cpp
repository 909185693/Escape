// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "OnlineSubsystemEscapeModule.h"
#include "OnlineSubsystemEscape.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemEscapePackage.h"


#define LOCTEXT_NAMESPACE "FOnlineSubsystemEscapeModule"

/**
* Class responsible for creating instance(s) of the subsystem
*/
class FOnlineFactoryEscape : public IOnlineFactory
{

private:

	/** Single instantiation of the Escape interface */
	static FOnlineSubsystemEscapePtr EscapeSingleton;

	virtual void DestroySubsystem()
	{
		if (EscapeSingleton.IsValid())
		{
			EscapeSingleton->Shutdown();
			EscapeSingleton = NULL;
		}
	}

public:

	FOnlineFactoryEscape() {}
	virtual ~FOnlineFactoryEscape()
	{
		DestroySubsystem();
	}

	virtual IOnlineSubsystemPtr CreateSubsystem(FName InstanceName)
	{
		if (!EscapeSingleton.IsValid())
		{
			EscapeSingleton = MakeShared<FOnlineSubsystemEscape, ESPMode::ThreadSafe>(InstanceName);
			if (EscapeSingleton->IsEnabled())
			{
				if (!EscapeSingleton->Init())
				{
					UE_LOG_ONLINE(Warning, TEXT("Escape API failed to initialize!"));
					DestroySubsystem();
				}
			}
			else
			{
				UE_LOG_ONLINE(Warning, TEXT("Escape API disabled!"));
				DestroySubsystem();
			}

			return EscapeSingleton;
		}

		UE_LOG_ONLINE(Warning, TEXT("Can't create more than one instance of Escape online subsystem!"));
		return NULL;
	}
};

FOnlineSubsystemEscapePtr FOnlineFactoryEscape::EscapeSingleton = NULL;

void FOnlineSubsystemEscapeModule::StartupModule()
{
	EscapeFactory = new FOnlineFactoryEscape();

	FOnlineSubsystemModule& OSS = FModuleManager::GetModuleChecked<FOnlineSubsystemModule>("OnlineSubsystem");
	OSS.RegisterPlatformService(ESCAPE_SUBSYSTEM, EscapeFactory);
}

void FOnlineSubsystemEscapeModule::ShutdownModule()
{
	FOnlineSubsystemModule& OSS = FModuleManager::GetModuleChecked<FOnlineSubsystemModule>("OnlineSubsystem");
	OSS.UnregisterPlatformService(ESCAPE_SUBSYSTEM);

	delete EscapeFactory;
	EscapeFactory = NULL;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FOnlineSubsystemEscapeModule, OnlineSubsystemEscape)