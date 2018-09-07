// Copyright 2018 by January. All Rights Reserved.

#include "EscapeCharacter_Lobby.h"


// Sets default values
AEscapeCharacter_Lobby::AEscapeCharacter_Lobby()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

bool AEscapeCharacter_Lobby::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
{
	return Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);
	//return Controller == RealViewer;
}

// Called when the game starts or when spawned
void AEscapeCharacter_Lobby::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEscapeCharacter_Lobby::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEscapeCharacter_Lobby::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

