// Copyright 2018 by January. All Rights Reserved.

#include "EscapeCharacter.h"
#include "EscapeCharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


AEscapeCharacter::AEscapeCharacter(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UEscapeCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// ÉãÏñ»ú¸Ë
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	if (CameraBoom != nullptr)
	{
		CameraBoom->SetupAttachment(RootComponent);
		CameraBoom->bUsePawnControlRotation = true;
		CameraBoom->CameraLagSpeed = 5.f;
		CameraBoom->CameraLagMaxDistance = 15.f;
	}

	// ÉãÏñ»ú¸úËæ
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	if (FollowCamera != nullptr)
	{
		FollowCamera->SetupAttachment(CameraBoom);
		FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	}

	PrimaryActorTick.bCanEverTick = true;
}

void AEscapeCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AEscapeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEscapeCharacter::Attack()
{

}

void AEscapeCharacter::StopAttacking()
{

}