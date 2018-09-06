// Copyright 2018 by January. All Rights Reserved.

#include "EscapeGameInstance.h"
#include "CrasheyeHelper.h"
#include "Engine/PendingNetGame.h"


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

void UEscapeGameInstance::InitializeGameInstance()
{
	FURL URL;
	URL.Protocol = TEXT("unreal");
	URL.Port = 8899;
	URL.Op.Add(TEXT("Name=Player"));
	URL.Op.Add(TEXT("Game=Game"));
	URL.Map = TEXT("/Game/Maps/EscapeMap");

	FString Error;

	// Creates the world context. This should be the only WorldContext that ever gets created for this GameInstance.
	WorldContext = &GetEngine()->CreateNewWorldContext(EWorldType::Game);
	check(WorldContext);
	WorldContext->OwningGameInstance = this;
	WorldContext->LastURL = URL;
	WorldContext->LastRemoteURL = URL;

	// In standalone create a dummy world from the beginning to avoid issues of not having a world until LoadMap gets us our real world
	UWorld* DummyWorld = UWorld::CreateWorld(EWorldType::Game, false);
	check(DummyWorld);
	DummyWorld->SetGameInstance(this);

	WorldContext->SetCurrentWorld(DummyWorld);

	UEngine* const Engine = GetEngine();
	check(Engine);
	if (Engine->LoadMap(*WorldContext, URL, nullptr, Error))
	{
		UE_LOG(LogTemp, Warning, TEXT("Create new world success!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Create new world failure. [%s]"), *Error);
	}

	UE_LOG(LogTemp, Log, TEXT("Create a new game world [%s]"), *GetNameSafe(DummyWorld));
}


