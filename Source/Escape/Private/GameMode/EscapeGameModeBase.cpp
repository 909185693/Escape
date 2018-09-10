// Copyright 2018 by January. All Rights Reserved.

#include "EscapeGameModeBase.h"
#include "Escape.h"
#include "EscapeGameSession.h"
#include "EscapePlayerController.h"
#include "Engine/NetworkDelegates.h"


AEscapeGameModeBase::AEscapeGameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// …Ë÷√ƒ¨»œΩ«…´¿∂Õº
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassFinder(TEXT("/Game/Blueprints/Character/EscapeCharacter_Player"));
	if (DefaultPawnClassFinder.Succeeded())
	{
		DefaultPawnClass = DefaultPawnClassFinder.Class;
	}

	// …Ë÷√Ω«…´øÿ÷∆¿∂Õº
	PlayerControllerClass = AEscapePlayerController::StaticClass();
}

bool AEscapeGameModeBase::Listen(FURL& InURL)
{
#if WITH_SERVER_CODE
	UWorld* World = GetWorld();

	if (NetDriver)
	{
		UE_LOG(LogTemp, Log, TEXT(""));
		return false;
	}

	// Create net driver.
	if (GEngine->CreateNamedNetDriver(World, NAME_EscapeNetDriver, NAME_EscapeNetDriver))
	{
		NetDriver = GEngine->FindNamedNetDriver(World, NAME_EscapeNetDriver);
		NetDriver->SetWorld(World);
	}

	if (NetDriver == NULL)
	{
		UE_LOG(LogTemp, Log, TEXT(""));
		return false;
	}

	FString Error;
	if (!NetDriver->InitListen(this, InURL, false, Error))
	{
		UE_LOG(LogTemp, Log, TEXT("Failed to listen: %s"), *Error);
		return false;
	}

	return true;
#else
	return false;
#endif // WITH_SERVER_CODE
}

void AEscapeGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AEscapeGameModeBase::StartPlay()
{
	Super::StartPlay();

	FURL URL;
	URL.Protocol = TEXT("escape");
	URL.Port = 8800;

	Listen(URL);
}

/** Returns game session class to use */
TSubclassOf<AGameSession> AEscapeGameModeBase::GetGameSessionClass() const
{
	return AEscapeGameSession::StaticClass();
}

EAcceptConnection::Type AEscapeGameModeBase::NotifyAcceptingConnection()
{
	check(NetDriver);
	if (NetDriver->ServerConnection)
	{
		// We are a client and we don't welcome incoming connections.
		UE_LOG(LogNet, Log, TEXT("NotifyAcceptingConnection: Client refused"));
		return EAcceptConnection::Reject;
	}
	else
	{
		// Server is up and running.
		UE_LOG(LogNet, Log, TEXT("NotifyAcceptingConnection: Server %s accept"), *GetName());
		return EAcceptConnection::Accept;
	}

	//WorldContext.PendingNetGame = NewObject<UPendingNetGame>();
	//WorldContext.PendingNetGame->Initialize(URL);
	//WorldContext.PendingNetGame->InitNetDriver();
	//if (!WorldContext.PendingNetGame->NetDriver)
	//{
	//	// UPendingNetGame will set the appropriate error code and connection lost type, so
	//	// we just have to propagate that message to the game.
	//	BroadcastTravelFailure(WorldContext.World(), ETravelFailure::PendingNetGameCreateFailure, WorldContext.PendingNetGame->ConnectionError);
	//	WorldContext.PendingNetGame = NULL;
	//	return EBrowseReturnVal::Failure;
	//}
}

void AEscapeGameModeBase::NotifyAcceptedConnection(class UNetConnection* Connection)
{
	check(NetDriver != NULL);
	check(NetDriver->ServerConnection == NULL);
	UE_LOG(LogNet, Log, TEXT("NotifyAcceptedConnection: Name: %s, TimeStamp: %s, %s"), *GetName(), FPlatformTime::StrTimestamp(), *Connection->Describe());
}

bool AEscapeGameModeBase::NotifyAcceptingChannel(class UChannel* Channel)
{
	check(Channel);
	check(Channel->Connection);
	check(Channel->Connection->Driver);
	UNetDriver* Driver = Channel->Connection->Driver;

	if (Driver->ServerConnection)
	{
		// We are a client and the server has just opened up a new channel.
		//UE_LOG(LogWorld, Log,  "NotifyAcceptingChannel %i/%i client %s", Channel->ChIndex, Channel->ChType, *GetName() );
		if (Channel->ChType == CHTYPE_Actor)
		{
			// Actor channel.
			//UE_LOG(LogWorld, Log,  "Client accepting actor channel" );
			return 1;
		}
		else if (Channel->ChType == CHTYPE_Voice)
		{
			// Accept server requests to open a voice channel, allowing for custom voip implementations
			// which utilize multiple server controlled voice channels.
			//UE_LOG(LogNet, Log,  "Client accepting voice channel" );
			return 1;
		}
		else
		{
			// Unwanted channel type.
			UE_LOG(LogNet, Log, TEXT("Client refusing unwanted channel of type %i"), (uint8)Channel->ChType);
			return 0;
		}
	}
	else
	{
		// We are the server.
		if (Channel->ChIndex == 0 && Channel->ChType == CHTYPE_Control)
		{
			// The client has opened initial channel.
			UE_LOG(LogNet, Log, TEXT("NotifyAcceptingChannel Control %i server %s: Accepted"), Channel->ChIndex, *GetFullName());
			return 1;
		}
		else if (Channel->ChType == CHTYPE_File)
		{
			// The client is going to request a file.
			UE_LOG(LogNet, Log, TEXT("NotifyAcceptingChannel File %i server %s: Accepted"), Channel->ChIndex, *GetFullName());
			return 1;
		}
		else
		{
			// Client can't open any other kinds of channels.
			UE_LOG(LogNet, Log, TEXT("NotifyAcceptingChannel %i %i server %s: Refused"), (uint8)Channel->ChType, Channel->ChIndex, *GetFullName());
			return 0;
		}
	}
}

void AEscapeGameModeBase::NotifyControlMessage(UNetConnection* Connection, uint8 MessageType, class FInBunch& Bunch)
{
	UE_LOG(LogTemp, Log, TEXT("Connection [%s] MessageType[%d]"), *GetNameSafe(Connection), MessageType);
}

