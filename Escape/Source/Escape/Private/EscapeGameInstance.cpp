// Copyright 2018 by January. All Rights Reserved.

#include "EscapeGameInstance.h"
#include "CrasheyeHelper.h"
#include "EscapeSystem.h"
#include "EscapeNetwork.h"
#include "EscapeEngine.h"
#include "Escape.h"


UEscapeGameInstance::UEscapeGameInstance(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, EscapeClientClassName(TEXT("/Script/EscapeNetwork.EscapeClient"))
{

}

class AEscapeGameSession* UEscapeGameInstance::GetGameSession() const
{
	return nullptr;
}

void UEscapeGameInstance::Init()
{
	Super::Init();

	if (!IsDedicatedServerInstance())
	{
		EscapeClientClass = LoadClass<UEscapeClient>(NULL, *EscapeClientClassName, NULL, LOAD_None, NULL);

		if (EscapeClientClass != nullptr)
		{
			if (EscapeClient == nullptr)
			{
				EscapeClient = NewObject<UEscapeClient>(GetTransientPackage(), EscapeClientClass);
			}

			if (EscapeClient != nullptr)
			{
				EscapeClient->Register(Cast<UEscapeEngine>(GetEngine()));

				EscapeSystem = NewObject<UEscapeSystem>(GetTransientPackage(), UEscapeSystem::StaticClass());
				EscapeSystem->Register(EscapeClient);
			}
		}
		else
		{
			UE_LOG(LogEscape, Error, TEXT("Failed to load class '%s'"), *EscapeClientClassName);
		}
	}
}

void UEscapeGameInstance::OnStart()
{
	Super::OnStart();

	// game requires the ability to ID users.
	const auto OnlineSub = IOnlineSubsystem::Get();
	check(OnlineSub);
	const auto IdentityInterface = OnlineSub->GetIdentityInterface();
	check(IdentityInterface.IsValid());

	UCrasheyeHelper::CrasheyeSetUserIdentifier(TEXT("TestCrasheye"));
	UCrasheyeHelper::CrasheyeSetAppVersion(TEXT("1.0.0"));
	
	UE_LOG(LogTemp, Log, TEXT("OnlineSub [%s] IdentityInterface [%s]"), *OnlineSub->GetSubsystemName().ToString(), *IdentityInterface->GetPlayerNickname(0));
}

class UEscapeClient* UEscapeGameInstance::GetEscapeClient() const
{
	return EscapeClient;
}

class UEscapeSystem* UEscapeGameInstance::GetEscapeSystem() const
{
	return EscapeSystem;
}