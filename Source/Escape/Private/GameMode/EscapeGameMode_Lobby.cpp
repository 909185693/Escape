// Copyright 2018 by January. All Rights Reserved.

#include "EscapeGameMode_Lobby.h"
#include "Escape.h"
#include "EscapePlayerState_Lobby.h"
#include "EscapePlayerController_Lobby.h"
#include "EscapeMessageContrller.h"

#include "EscapeServer.h"
#include "EscapeClient.h"


AEscapeGameMode_Lobby::AEscapeGameMode_Lobby(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EscapeServerClassName = TEXT("/Script/EscapeNetwork.EscapeServer");

	// 设置默认角色类
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassFinder(TEXT("/Game/Blueprints/Character/EscapeCharacter_Lobby"));
	if (DefaultPawnClassFinder.Succeeded())
	{
		DefaultPawnClass = DefaultPawnClassFinder.Class;
	}

	// 设置玩家状态类
	PlayerStateClass = AEscapePlayerState_Lobby::StaticClass();

	// 设置角色控制类
	PlayerControllerClass = AEscapePlayerController_Lobby::StaticClass();
}

template <typename UserClass>
typedef void(UserClass::MessageCallbackPtr*)(void*, EErrorCode);

template <typename UserClass>
inline void AddMessageCallback2(UserClass* InUserObject, MessageCallbackPtr InFunc)
{
	//FMessageCallback MessageCallback;
	////MessageCallback.MessageDelegate.BindUObject(InUserObject, InFunc);

	//MessageCallbacks.Add(MessageCallback);
}

void AEscapeGameMode_Lobby::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	if (GetNetMode() == NM_DedicatedServer)
	{
		EscapeServerClass = LoadClass<UEscapeServer>(NULL, *EscapeServerClassName, NULL, LOAD_None, NULL);

		if (EscapeServerClass)
		{
			if (EscapeServer == nullptr)
			{
				EscapeServer = NewObject<UEscapeServer>(GetTransientPackage(), EscapeServerClass);
			}

			if (EscapeServer != nullptr)
			{
				EscapeServer->Run();
			}
		}
		else
		{
			UE_LOG(LogEscape, Error, TEXT("Failed to load class '%s'"), *EscapeServerClassName);
		}

		UEscapeMessageContrller* EscapeMessageContrller = NewObject<UEscapeMessageContrller>(GetTransientPackage(), UEscapeMessageContrller::StaticClass());

		EscapeServer->AddMessageCallback(EscapeMessageContrller, &UEscapeMessageContrller::UserLoginCallback);
	}
}