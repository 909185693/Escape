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

		if (Role == ROLE_AutonomousProxy)
		{
			ServerAttack(AttackCount);
		}

		if (Role == ROLE_Authority)
		{
			BroadcastAttack(AttackCount);
		}

		if (IsLocallyControlled())
		{
			bIsAttacking = true;

			AttackCount = ++AttackCount % ComboTable.Num();
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

bool AEscapeCharacter::ServerAttack_Validate(uint8 Count)
{
	return true;
}

void AEscapeCharacter::ServerAttack_Implementation(uint8 Count)
{
	AttackCount = Count;

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

void AEscapeCharacter::ManagerCharactorRotation(float DeltaSeconds)
{
	UEscapeCharacterMovementComponent* EscapeCharacterMovement = Cast<UEscapeCharacterMovementComponent>(GetCharacterMovement());
	if (EscapeCharacterMovement != nullptr)
	{
		if (EscapeCharacterMovement->IsMovingOnGround())
		{

		}
	}
}

void AEscapeCharacter::SetCharactorRotation(const FRotator& NewTargetRotation, bool bInterp, float InterpSpeed)
{
	TargetRotation = NewTargetRotation;

	if (bInterp)
	{
		const float DeltaSeconds = GetWorld()->GetDeltaSeconds();

		FMath::RInterpTo(CharacterRotation, TargetRotation, DeltaSeconds, InterpSpeed);
	}
	else
	{
		CharacterRotation = NewTargetRotation;
	}

	SetActorRotation(CharacterRotation);

	if (Role == ROLE_AutonomousProxy)
	{
		ServerSetCharactorRotation(TargetRotation, CharacterRotation);
	}
}

bool AEscapeCharacter::ServerSetCharactorRotation_Validate(const FRotator& NewTargetRotation, const FRotator& NewCharactorRotation)
{
	return true;
}

void AEscapeCharacter::ServerSetCharactorRotation_Implementation(const FRotator& NewTargetRotation, const FRotator& NewCharactorRotation)
{
	TargetRotation = NewTargetRotation;
	CharacterRotation = NewCharactorRotation;
}

//void AEscapeCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//}