// Copyright 2018 by January. All Rights Reserved.

#include "EscapeGameSession.h"
#include "EscapeEngine.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystemSessionSettings.h"


AEscapeGameSession::AEscapeGameSession(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, EscapeClientClassName(TEXT("/Script/EscapeNetwork.EscapeClient"))
{

}

void AEscapeGameSession::Destroyed()
{
#if ESCAPE_BUILD_SERVER
	if (EscapeClient != nullptr)
	{
		EscapeClient->ClearMessageCallback(this);
	}
#endif

	Super::Destroyed();
}

void AEscapeGameSession::InitOptions(const FString& Options)
{
	Super::InitOptions(Options);

#if ESCAPE_BUILD_SERVER
	Guid = UGameplayStatics::ParseOption(Options, TEXT("guid"));
	bIsLogicServer = UGameplayStatics::HasOption(Options, TEXT("IsLogicServer"));
#endif
}

void AEscapeGameSession::RegisterServer()
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr SessionInt = Online::GetSessionInterface();
		if (SessionInt.IsValid())
		{
			//TSharedPtr<class FShooterOnlineSessionSettings> ShooterHostSettings = MakeShareable(new FShooterOnlineSessionSettings(false, false, 16));
			//ShooterHostSettings->Set(SETTING_MATCHING_HOPPER, FString("TeamDeathmatch"), EOnlineDataAdvertisementType::DontAdvertise);
			//ShooterHostSettings->Set(SETTING_MATCHING_TIMEOUT, 120.0f, EOnlineDataAdvertisementType::ViaOnlineService);
			//ShooterHostSettings->Set(SETTING_SESSION_TEMPLATE_NAME, FString("GameSession"), EOnlineDataAdvertisementType::DontAdvertise);
			//ShooterHostSettings->Set(SETTING_MAPNAME, GetWorld()->GetMapName(), EOnlineDataAdvertisementType::ViaOnlineService);
			//ShooterHostSettings->bAllowInvites = true;
			//ShooterHostSettings->bIsDedicated = true;
			//HostSettings = ShooterHostSettings;
			//OnCreateSessionCompleteDelegateHandle = SessionInt->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
			//SessionInt->CreateSession(0, NAME_GameSession, *HostSettings);
		}
	}

#if ESCAPE_BUILD_SERVER
	if (!bIsLogicServer)
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
				UGameInstance* GameInstance = GetGameInstance();
				UEscapeEngine* EscapeEngine = GameInstance ? Cast<UEscapeEngine>(GameInstance->GetEngine()) : nullptr;

				EscapeClient->Register(EscapeEngine);
				EscapeClient->AddMessageCallback(ELogicCode::CONNECTION, this, &AEscapeGameSession::NotifyConnection);
				EscapeClient->AddMessageCallback(ELogicCode::REGISTER_SERVER, this, &AEscapeGameSession::NotifyRegisterServer);
			}
		}
		else
		{
			UE_LOG(LogEscape, Error, TEXT("Failed to load class '%s'"), *EscapeClientClassName);
		}
	}
#endif
}

#if ESCAPE_BUILD_SERVER
void AEscapeGameSession::NotifyConnection(void* Data, EErrorCode Error)
{
	if (Error == EErrorCode::NONE)
	{
		if (EscapeClient != nullptr)
		{
			FDedicatedServer DedicatedServer;

			const FURL& URL = GetWorld()->URL;

			DedicatedServer.Port = URL.Port;

			FPlatformString::Strncpy(DedicatedServer.IP, "127.0.0.1", sizeof(DedicatedServer.IP));
			FPlatformString::Strncpy(DedicatedServer.Guid, TCHAR_TO_ANSI(*Guid), sizeof(DedicatedServer.Guid));

			EscapeClient->Send(ELogicCode::REGISTER_SERVER, sizeof(FDedicatedServer), &DedicatedServer);

			UE_LOG(LogEscape, Log, TEXT("Start register this dedicated server to logic server Guid[%s] %s:%d"), *Guid, *URL.Host, URL.Port);
		}
	}
}

void AEscapeGameSession::NotifyRegisterServer(void* Data, EErrorCode Error)
{

}
#endif
