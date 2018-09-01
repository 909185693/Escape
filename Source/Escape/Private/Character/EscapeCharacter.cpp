// Copyright 2018 by January. All Rights Reserved.

#include "EscapeCharacter.h"
#include "EscapeCharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"


AEscapeCharacter::AEscapeCharacter(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UEscapeCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
	, AttackSpeed (1.f)
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

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

	bUseControllerRotationYaw = false;

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
	if (!bIsAttacking)
	{
		if (ComboTable.IsValidIndex(AttackCount))
		{
			PlayAnimMontage(ComboTable[AttackCount].Montage, AttackSpeed);
		}

		if (Role != ROLE_SimulatedProxy)
		{
			bIsAttacking = true;

			AttackCount = ++AttackCount % ComboTable.Num();
		}

		if (Role == ROLE_AutonomousProxy)
		{
			ServerAttack();
		}

		if (Role == ROLE_Authority)
		{
			BroadcastAttack(AttackCount);
		}
	}
	else
	{
		bSaveAttack = true;
	}
}

void AEscapeCharacter::StopAttacking()
{

}

void AEscapeCharacter::ComboAttackSave()
{
	if (bSaveAttack)
	{
		bSaveAttack = false;

		bIsAttacking = false;

		Attack();
	}
}

void AEscapeCharacter::ResetCombo()
{
	AttackCount = 0;

	bSaveAttack = false;

	bIsAttacking = false;
}

bool AEscapeCharacter::ServerAttack_Validate()
{
	return true;
}

void AEscapeCharacter::ServerAttack_Implementation()
{
	Attack();
}

void AEscapeCharacter::BroadcastAttack_Implementation(uint8 Count)
{
	if (Role == ROLE_SimulatedProxy)
	{
		AttackCount = Count;

		Attack();
	}
}

//void AEscapeCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//}