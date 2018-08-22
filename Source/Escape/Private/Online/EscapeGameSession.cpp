// Copyright 2018 by January. All Rights Reserved.

#include "EscapeGameSession.h"
#include "OnlineSubsystemSessionSettings.h"

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
}
