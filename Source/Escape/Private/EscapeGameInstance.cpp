// Copyright 2018 by January. All Rights Reserved.

#include "EscapeGameInstance.h"
#include "CrasheyeHelper.h"
#include "EscapeEngine.h"


UEscapeGameInstance::UEscapeGameInstance(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)

{

}

class AEscapeGameSession* UEscapeGameInstance::GetGameSession() const
{
	return nullptr;
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
