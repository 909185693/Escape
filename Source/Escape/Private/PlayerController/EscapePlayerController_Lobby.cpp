// Copyright 2018 by January. All Rights Reserved.

#include "EscapePlayerController_Lobby.h"
#include "EscapeGameInstance.h"


void AEscapePlayerController_Lobby::StartGame()
{
	JionGameRoom();
}

bool AEscapePlayerController_Lobby::JionGameRoom_Validate()
{
	return true;
}

void AEscapePlayerController_Lobby::JionGameRoom_Implementation()
{
	if (IsRunningDedicatedServer())
	{
		FString GameNameOrProjectFile;
		if (FPaths::IsProjectFilePathSet())
		{
			GameNameOrProjectFile = FString::Printf(TEXT("\"%s\""), *FPaths::GetProjectFilePath());
		}
		else
		{
			GameNameOrProjectFile = FApp::GetProjectName();
		}

		FString Params = FString::Printf(TEXT("%s %s -server -log"),
			*GameNameOrProjectFile,
			TEXT("/Game/Maps/EscapeMap"));

		// launch the game process
		FString GamePath = FPlatformProcess::GenerateApplicationPath(FApp::GetName(), FApp::GetBuildConfiguration());

		uint32 ProcessID = 0;
		FProcHandle ProcessHandle = FPlatformProcess::CreateProc(*GamePath, *Params, true, false, false, &ProcessID, 0, NULL, NULL);

		if (!ProcessHandle.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to run a copy of the game on this PC."));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Start room server success!"));
		}
	}
}

