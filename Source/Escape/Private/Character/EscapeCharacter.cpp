// Copyright 2018 by January. All Rights Reserved.

#include "EscapeCharacter.h"
#include "Escape.h"
#include "EscapeCharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


AEscapeCharacter::AEscapeCharacter(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UEscapeCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
	, AttackSpeed(1.f)
	, TurnInPlaceDelay(0.f)
{
	GetMesh()->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickMontagesWhenNotRendered;

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

	// ÎäÆ÷Åö×²
	WeaponCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCapsule"));
	if (WeaponCapsule != nullptr)
	{
		WeaponCapsule->InitCapsuleSize(10.728559f, 81.710457f);
		WeaponCapsule->SetRelativeLocationAndRotation(FVector(0.029829f, -57.326813f, 2.027954f), FRotator(0.000000f, -179.955994f, -89.350052));
		WeaponCapsule->SetCollisionProfileName(TEXT("NoCollision"));
		WeaponCapsule->SetupAttachment(GetMesh(), TEXT("weapon_r"));
	}

	bUseControllerRotationYaw = false;

	PrimaryActorTick.bCanEverTick = true;
}

void AEscapeCharacter::BeginPlay()
{
	Super::BeginPlay();

	const FRotator& ActorRotation = GetActorRotation();

	TargetRotation = ActorRotation;
	CharacterRotation = ActorRotation;
}

TEnumAsByte<ECardinalDirection::Type> AEscapeCharacter::ConvertDirection(float NewDirection) const
{
	if (NewDirection >= -45.f && NewDirection < 45.f)
	{
		return ECardinalDirection::Forwards;
	}
	else if (NewDirection >= 45.f && NewDirection < 135.f)
	{
		return ECardinalDirection::Rightwards;
	}
	else if (NewDirection >= -135.f && NewDirection < -45.f)
	{
		return ECardinalDirection::Leftwards;
	}
	else
	{
		return ECardinalDirection::Backwards;
	}
}

float AEscapeCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	APawn* InstigatorPawn = EventInstigator ? EventInstigator->GetPawn() : nullptr;

	FHitResult HitResult;
	FVector ImpulseDir;

	DamageEvent.GetBestHitInfo(this, InstigatorPawn, HitResult, ImpulseDir);

	PlayHit(ActualDamage, InstigatorPawn, DamageCauser, HitResult);

	return ActualDamage;
}

void AEscapeCharacter::PlayHit_Implementation(float DamageAmount, APawn* InstigatorPawn, AActor* DamageCauser, const FHitResult& HiResult)
{
	const FRotator& HitFromRotation = GetActorTransform().InverseTransformPosition(HiResult.ImpactPoint).Rotation();

	TEnumAsByte<ECardinalDirection::Type> HitFromCardinalDirection = ConvertDirection(HitFromRotation.Yaw);
	
	UAnimMontage** MontageToPlay = GithitMontage.Find(HitFromCardinalDirection);
	if (MontageToPlay != nullptr)
	{
		PlayAnimMontage(*MontageToPlay);

		UE_LOG(LogTemp, Log, TEXT("PlayHit [%s]"), *GetNameSafe(*MontageToPlay));
	}
}

void AEscapeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ManagerCharactorRotation(DeltaTime);
	
	//CustomTimeDilation = FMath::FInterpTo(CustomTimeDilation, 1.f, DeltaTime, 128.f);
	CustomTimeDilation = FMath::FInterpConstantTo(CustomTimeDilation, 1.f, DeltaTime, TimeDilationInterpSpeed);
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

void AEscapeCharacter::DamagedClear()
{
	DamagedActors.Empty();

	LastWeaponLocation = WeaponCapsule->GetComponentLocation();
}

void AEscapeCharacter::DamageCheck(float DeltaTime)
{
	const FVector& TraceStart = LastWeaponLocation;
	const FVector& TraceEnd = WeaponCapsule->GetComponentLocation();

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.bReturnPhysicalMaterial = true;
	CollisionQueryParams.AddIgnoredActor(this);

	TArray<FHitResult> HitResults;
	GetWorld()->SweepMultiByChannel(HitResults, TraceStart, TraceEnd, WeaponCapsule->GetComponentQuat(), ECC_Weapon, WeaponCapsule->GetCollisionShape(), CollisionQueryParams);

	for (const FHitResult& HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor != nullptr && !DamagedActors.Contains(HitActor))
		{
			CustomTimeDilation = 0.001f;

			DamagedActors.AddUnique(HitActor);

			PlayImpactEffect(HitResult);

			if (Role == ROLE_Authority && ComboTable.IsValidIndex(AttackCount))
			{
				FVector HitFromDirection = (HitResult.ImpactPoint - HitActor->GetActorLocation()).GetSafeNormal();

				UGameplayStatics::ApplyPointDamage(HitActor, ComboTable[AttackCount].Damage, HitFromDirection, HitResult, Controller, this, ComboTable[AttackCount].DamageType);
			}

			//DrawDebugLine(GetWorld(), HitResult.ImpactPoint, HitResult.ImpactPoint + 50.f * HitResult.ImpactNormal, FColor::Yellow, false, 10.f);
			//DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.f, FColor::Red, false, 10.f);
			//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("HitActor [%s]"), *GetNameSafe(HitActor)), true, false, FLinearColor::White, 10.f);
		}
	}

	LastWeaponLocation = WeaponCapsule->GetComponentLocation();
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

void AEscapeCharacter::PlayImpactEffect(const FHitResult& HitResult)
{
	TEnumAsByte<EPhysicalSurface> PhysicalSurface = HitResult.PhysMaterial.IsValid() ? HitResult.PhysMaterial->SurfaceType : TEnumAsByte<EPhysicalSurface>(EPhysicalSurface::SurfaceType_Default);
	
	UParticleSystem** ParticleSystem = ImpactParticle.Find(PhysicalSurface);
	if (ParticleSystem != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), *ParticleSystem, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());
	}
}

void AEscapeCharacter::ManagerCharactorRotation(float DeltaTime)
{
	if (Role == ROLE_SimulatedProxy)
	{
		return;
	}

	UEscapeCharacterMovementComponent* EscapeCharacterMovement = Cast<UEscapeCharacterMovementComponent>(GetCharacterMovement());
	if (EscapeCharacterMovement != nullptr)
	{
		if (EscapeCharacterMovement->IsMovingOnGround())
		{
			if (EscapeCharacterMovement->GetCurrentAcceleration().SizeSquared() > 0.f)
			{
				const FRotator& NewTatgetRotation = FRotator(0.f, GetControlRotation().Yaw, 0.f).GetNormalized();

				SetCharactorRotation(NewTatgetRotation, true, 15.f);

				LastVelocityRotation = EscapeCharacterMovement->Velocity.Rotation();
			}
			else
			{
				class UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
				if (AnimInstance != nullptr)
				{
					//const float DistanceCurve = 0.f;
					//if (AnimInstance->GetCurveValue(FName("DistanceCurve"), DistanceCurve))
					//{

					//}
				}
			}
		}
		else if (EscapeCharacterMovement->IsFalling())
		{
			const FRotator& NewTatgetRotation = FRotator(0.f, GetControlRotation().Yaw, 0.f).GetNormalized();

			const float InterpSpeed = FMath::Abs(CharacterRotation.Yaw - NewTatgetRotation.Yaw) / 10.f;

			SetCharactorRotation(NewTatgetRotation, true, InterpSpeed);
		}
	}
}

void AEscapeCharacter::SetCharactorRotation(const FRotator& NewTargetRotation, bool bInterp, float InterpSpeed)
{
	TargetRotation = NewTargetRotation;

	if (bInterp)
	{
		const float DeltaSeconds = GetWorld()->GetDeltaSeconds();

		CharacterRotation = FMath::RInterpTo(CharacterRotation, TargetRotation, DeltaSeconds, InterpSpeed);
	}
	else
	{
		CharacterRotation = NewTargetRotation;
	}

	SetActorRotation(CharacterRotation);

	//if (Role == ROLE_AutonomousProxy)
	//{
	//	ServerSetCharactorRotation(TargetRotation, CharacterRotation);
	//}
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