// Copyright 2018 by January. All Rights Reserved.

#include "EscapeCharacter.h"
#include "Escape.h"
#include "EscapeGameMode_Game.h"
#include "EscapePlayerController_Game.h"
#include "EscapeAttributeSet.h"
#include "EscapeAbilitySystemComponent.h"
#include "EscapeCharacterMovementComponent.h"
#include "EscapeWeapon.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "UnrealNetwork.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


AEscapeCharacter::AEscapeCharacter(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UEscapeCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
	, TurnInPlaceDelay(0.f)
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	// Create ability system component, and set it to be explicitly replicated
	AbilitySystemComponent = CreateDefaultSubobject<UEscapeAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// Create the attribute set, this replicates by default
	AttributeSet = CreateDefaultSubobject<UEscapeAttributeSet>(TEXT("AttributeSet"));

	// 摄像机杆
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	if (CameraBoom != nullptr)
	{
		CameraBoom->SetupAttachment(RootComponent);
		CameraBoom->bUsePawnControlRotation = true;
		CameraBoom->CameraLagSpeed = 5.f;
		CameraBoom->CameraLagMaxDistance = 15.f;
	}

	// 摄像机跟随
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	if (FollowCamera != nullptr)
	{
		FollowCamera->SetupAttachment(CameraBoom);
		FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	}

	DamagedPauseFPSTime = 0.1f;

	LaunchSpeed = 1800.f;

	bUseControllerRotationYaw = false;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = ETickingGroup::TG_PrePhysics;
}

void AEscapeCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (Role == ROLE_Authority)
	{
		EquipWeapon(WeaponClass);
	}
}

void AEscapeCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsNetMode(NM_DedicatedServer))
	{
		GetMesh()->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;
	}
	else
	{
		GetMesh()->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	}

	const FRotator& ActorRotation = GetActorRotation();

	TargetRotation = ActorRotation;
	CharacterRotation = ActorRotation;
}

class UCapsuleComponent* AEscapeCharacter::GetWeaponCollision() const
{
	return Weapon ? Weapon->GetCapsuleComponent() : nullptr;
}

UAbilitySystemComponent* AEscapeCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AEscapeCharacter::EquipWeapon(TSoftClassPtr<AEscapeWeapon> NewWeaponClass)
{
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.Owner = Controller;
	ActorSpawnParameters.Instigator = this;
	ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AEscapeWeapon* NewWeapon = GetWorld()->SpawnActor<AEscapeWeapon>(WeaponClass.LoadSynchronous(), GetActorTransform(), ActorSpawnParameters);

	EquipWeapon(NewWeapon, Weapon);
}

void AEscapeCharacter::EquipWeapon(AEscapeWeapon* NewWeapon, AEscapeWeapon* LastWeapon)
{
	if (LastWeapon != nullptr)
	{
		LastWeapon->Destroy();
	}

	if (NewWeapon)
	{
		Weapon = NewWeapon;

		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);
		NewWeapon->SetMasterPoseComponent(GetMesh());
		NewWeapon->SetOwner(Controller);
		NewWeapon->Instigator = this;
	}
}

void AEscapeCharacter::OnRep_Weapon(AEscapeWeapon* LastWeapon)
{
	EquipWeapon(Weapon, LastWeapon);
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

void AEscapeCharacter::SetRagdollPhysics()
{
	bool bInRagdoll = false;

	if (IsPendingKill())
	{
		bInRagdoll = false;
	}
	else if (!GetMesh() || !GetMesh()->GetPhysicsAsset())
	{
		bInRagdoll = false;
	}
	else
	{
		// initialize physics/etc
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->WakeAllRigidBodies();
		GetMesh()->bBlendPhysics = true;

		bInRagdoll = true;
	}

	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->SetComponentTickEnabled(false);

	if (!bInRagdoll)
	{
		// hide and set short lifespan
		TurnOff();
		SetActorHiddenInGame(true);
		SetLifeSpan(1.0f);
	}
	else
	{
		SetLifeSpan(10.0f);
	}
}

void AEscapeCharacter::PlayHit_Implementation(float ActualDamage, bool bIsCrit, bool bKilled, APawn* PawnInstigator, AActor* DamageCauser, const FHitResult& HiResult)
{
	const FRotator& HitFromRotation = GetActorTransform().InverseTransformPosition(HiResult.ImpactPoint).Rotation();

	TEnumAsByte<ECardinalDirection::Type> HitFromCardinalDirection = ConvertDirection(HitFromRotation.Yaw);

	UAnimMontage** MontageToPlay = GithitMontage.Find(HitFromCardinalDirection);
	if (MontageToPlay != nullptr)
	{
		PlayAnimMontage(*MontageToPlay);
	}

	AEscapePlayerController_Game* PlayerController = PawnInstigator ? Cast<AEscapePlayerController_Game>(PawnInstigator->Controller) : nullptr;
	if (PlayerController != nullptr)
	{
		PlayerController->AddDamageInfo(ActualDamage, HiResult.ImpactPoint, bIsCrit);
	}

	if (bKilled)
	{
		OnDeath(ActualDamage, PawnInstigator, DamageCauser);
	}
}

void AEscapeCharacter::OnDeath(float KillingDamage, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	if (bIsDying)
	{
		return;
	}

	bReplicateMovement = false;
	TearOff();
	bIsDying = true;

	// cannot use IsLocallyControlled here, because even local client's controller may be NULL here
	if (GetNetMode() != NM_DedicatedServer && DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}

	DetachFromControllerPendingDestroy();

	if (GetMesh())
	{
		static FName CollisionProfileName(TEXT("Ragdoll"));
		GetMesh()->SetCollisionProfileName(CollisionProfileName);
	}
	SetActorEnableCollision(true);

	// Ragdoll
	if (DeathAnimationAsset != nullptr)
	{
		// Trigger ragdoll a little before the animation early so the character doesn't
		// blend back to its normal position.
		const float TriggerRagdollTime = 0.3f;

		// Enable blend physics so the bones are properly blending against the montage.
		GetMesh()->bBlendPhysics = true;
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		GetMesh()->PlayAnimation(DeathAnimationAsset, false);

		// Use a local timer handle as we don't need to store it for later but we don't need to look for something to clear
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AEscapeCharacter::SetRagdollPhysics, TriggerRagdollTime, false);
	}
	else
	{
		SetRagdollPhysics();
	}

	// disable collisions on capsule
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void AEscapeCharacter::Tick(float DeltaTime)
{
	TimeSeconds = GetWorld()->GetTimeSeconds();

	Super::Tick(DeltaTime);

	ManagerCharactorRotation(DeltaTime);

	if (TimeSeconds - LastDamagedPauseFPSTime > DamagedPauseFPSTime)
	{
		CustomTimeDilation = 1.f;

		GetMesh()->GlobalAnimRateScale = 1.f;
	}
}

void AEscapeCharacter::Attack()
{
	if (!bIsAttacking)
	{
		if (ComboTable.IsValidIndex(AttackCount))
		{
			DamagedClear();

			const float AttackSpeed = GetAttackSpeed();

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

		if (IsLocallyControlled() && ComboTable.Num() > 0)
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

	UCapsuleComponent* WeaponCollision = GetWeaponCollision();
	if (WeaponCollision != nullptr)
	{
		LastWeaponLocation = WeaponCollision->GetComponentLocation();
	}
}

void AEscapeCharacter::DamageCheck(float DeltaTime)
{
	UCapsuleComponent* WeaponCollision = GetWeaponCollision();
	if (WeaponCollision != nullptr)
	{
		if (Role == ROLE_SimulatedProxy)
		{
			return;
		}

		if (GetMesh()->SkeletalMesh == nullptr)
		{
			return;
		}

		const FVector& TraceStart = LastWeaponLocation;
		const FVector& TraceEnd = WeaponCollision->GetComponentLocation();

		//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("TraceEnd [%s] DeltaTime[%0.2f]"), *TraceEnd.ToCompactString(), DeltaTime), true, false, FLinearColor::White, 10.f);

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.bReturnPhysicalMaterial = true;
		CollisionQueryParams.AddIgnoredActor(this);

		TArray<FHitResult> HitResults;
		GetWorld()->SweepMultiByChannel(HitResults, TraceStart, TraceEnd, WeaponCollision->GetComponentQuat(), ECC_Weapon, WeaponCollision->GetCollisionShape(), CollisionQueryParams);

		for (const FHitResult& HitResult : HitResults)
		{
			AActor* HitActor = HitResult.GetActor();
			if (HitActor != nullptr && !DamagedActors.Contains(HitActor))
			{
				DamagedActors.AddUnique(HitActor);

				if (Role == ROLE_Authority && ComboTable.IsValidIndex(AttackCount))
				{
					PlayDamaged(HitResult);

					FVector HitFromDirection = HitResult.ImpactNormal.GetSafeNormal(); //(HitResult.ImpactPoint - HitActor->GetActorLocation()).GetSafeNormal();

					UGameplayStatics::ApplyPointDamage(HitActor, ComboTable[AttackCount].Damage, HitFromDirection, HitResult, Controller, this, ComboTable[AttackCount].DamageType);

					UE_LOG(LogTemp, Log, TEXT("AEscapeCharacter::DamageCheck() HitActor[%s]"), *GetNameSafe(HitActor));
				}

				//DrawDebugLine(GetWorld(), HitResult.ImpactPoint, HitResult.ImpactPoint + 50.f * HitResult.ImpactNormal, FColor::Yellow, false, 10.f);
				//DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.f, FColor::Red, false, 10.f);
				//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("HitActor [%s]"), *GetNameSafe(HitActor)), true, false, FLinearColor::White, 10.f);
			}
		}

		LastWeaponLocation = WeaponCollision->GetComponentLocation();
	}
}

void AEscapeCharacter::LaunchCharacter()
{
	FVector LaunchVelocity = GetActorRotation().Vector() * LaunchSpeed;
	if (!LastControlInputVector.IsZero())
	{
		LaunchVelocity = LastControlInputVector.Rotation().Vector() * LaunchSpeed;
	}
	
	LaunchCharacter(LaunchVelocity, true, false);
}

void AEscapeCharacter::LaunchCharacter(FVector LaunchVelocity, bool bXYOverride, bool bZOverride)
{
	if (Role == ROLE_AutonomousProxy)
	{
		ServerLaunchCharacter(LaunchVelocity, bXYOverride, bZOverride);
	}

	bIsAttacking = true;

	PlayAnimMontage(LaunchMontage);

	Super::LaunchCharacter(LaunchVelocity, bXYOverride, bZOverride);
}

bool AEscapeCharacter::ServerLaunchCharacter_Validate(FVector LaunchVelocity, bool bXYOverride, bool bZOverride)
{
	return true;
}

void AEscapeCharacter::ServerLaunchCharacter_Implementation(FVector LaunchVelocity, bool bXYOverride, bool bZOverride)
{
	LaunchCharacter(LaunchVelocity, bXYOverride, bZOverride);
	
	BroadcastLaunchCharacter(LaunchVelocity, bXYOverride, bZOverride);
}

void AEscapeCharacter::BroadcastLaunchCharacter_Implementation(FVector LaunchVelocity, bool bXYOverride, bool bZOverride)
{
	if (Role == ROLE_SimulatedProxy)
	{
		LaunchCharacter(LaunchVelocity, bXYOverride, bZOverride);
	}
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

void AEscapeCharacter::PlayDamaged_Implementation(const FHitResult& HitResult)
{
	LastDamagedPauseFPSTime = TimeSeconds;

	CustomTimeDilation = 0.f;

	GetMesh()->GlobalAnimRateScale = 0.f;

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

				const float InterpSpeed = FMath::Min(7.5f, FMath::Abs(CharacterRotation.Yaw - NewTatgetRotation.Yaw) / 10.f);

				SetCharactorRotation(NewTatgetRotation, true, InterpSpeed);

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

			const float InterpSpeed = FMath::Min(6.5f, FMath::Abs(CharacterRotation.Yaw - NewTatgetRotation.Yaw) / 10.f);

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

bool AEscapeCharacter::IsAlive() const
{
	return GetHealth() > 0.f;
}

float AEscapeCharacter::GetHealth() const
{
	return AttributeSet->GetHealth();
}

float AEscapeCharacter::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

float AEscapeCharacter::GetMana() const
{
	return AttributeSet->GetMana();
}

float AEscapeCharacter::GetMaxMana() const
{
	return AttributeSet->GetMaxMana();
}

float AEscapeCharacter::GetMoveSpeed() const
{
	return AttributeSet->GetMoveSpeed();
}

float AEscapeCharacter::GetAttackSpeed() const
{
	return AttributeSet->GetAttackSpeed();
}

int32 AEscapeCharacter::GetCharacterLevel() const
{
	//return CharacterLevel;
	return 0;
}

bool AEscapeCharacter::SetCharacterLevel(int32 NewLevel)
{
	//if (CharacterLevel != NewLevel && NewLevel > 0)
	//{
	//	// Our level changed so we need to refresh abilities
	//	RemoveStartupGameplayAbilities();
	//	CharacterLevel = NewLevel;
	//	AddStartupGameplayAbilities();

	//	return true;
	//}
	return false;
}

bool AEscapeCharacter::ActivateAbilitiesWithItemSlot(FEscapeItemSlot ItemSlot, bool bAllowRemoteActivation)
{
	FGameplayAbilitySpecHandle* FoundHandle = SlottedAbilities.Find(ItemSlot);

	if (FoundHandle && AbilitySystemComponent)
	{
		return AbilitySystemComponent->TryActivateAbility(*FoundHandle, bAllowRemoteActivation);
	}

	return false;
}

void AEscapeCharacter::GetActiveAbilitiesWithItemSlot(FEscapeItemSlot ItemSlot, TArray<UEscapeGameplayAbility*>& ActiveAbilities)
{
	FGameplayAbilitySpecHandle* FoundHandle = SlottedAbilities.Find(ItemSlot);

	if (FoundHandle && AbilitySystemComponent)
	{
		FGameplayAbilitySpec* FoundSpec = AbilitySystemComponent->FindAbilitySpecFromHandle(*FoundHandle);

		if (FoundSpec)
		{
			TArray<UGameplayAbility*> AbilityInstances = FoundSpec->GetAbilityInstances();

			// Find all ability instances executed from this slot
			for (UGameplayAbility* ActiveAbility : AbilityInstances)
			{
				ActiveAbilities.Add(Cast<UEscapeGameplayAbility>(ActiveAbility));
			}
		}
	}
}

bool AEscapeCharacter::ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation)
{
	if (AbilitySystemComponent)
	{
		return AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags, bAllowRemoteActivation);
	}

	return false;
}

void AEscapeCharacter::GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UEscapeGameplayAbility*>& ActiveAbilities)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GetActiveAbilitiesWithTags(AbilityTags, ActiveAbilities);
	}
}

bool AEscapeCharacter::GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration)
{
	if (AbilitySystemComponent && CooldownTags.Num() > 0)
	{
		TimeRemaining = 0.f;
		CooldownDuration = 0.f;

		FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTags);
		TArray< TPair<float, float> > DurationAndTimeRemaining = AbilitySystemComponent->GetActiveEffectsTimeRemainingAndDuration(Query);
		if (DurationAndTimeRemaining.Num() > 0)
		{
			int32 BestIdx = 0;
			float LongestTime = DurationAndTimeRemaining[0].Key;
			for (int32 Idx = 1; Idx < DurationAndTimeRemaining.Num(); ++Idx)
			{
				if (DurationAndTimeRemaining[Idx].Key > LongestTime)
				{
					LongestTime = DurationAndTimeRemaining[Idx].Key;
					BestIdx = Idx;
				}
			}

			TimeRemaining = DurationAndTimeRemaining[BestIdx].Key;
			CooldownDuration = DurationAndTimeRemaining[BestIdx].Value;

			return true;
		}
	}
	return false;
}

void AEscapeCharacter::HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, AEscapeCharacter* InstigatorPawn, AActor* DamageCauser)
{
	OnDamaged(DamageAmount, HitInfo, DamageTags, InstigatorPawn, DamageCauser);
}

void AEscapeCharacter::HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{
	// We only call the BP callback if this is not the initial ability setup
	if (bAbilitiesInitialized)
	{
		OnHealthChanged(DeltaValue, EventTags);
	}
}

void AEscapeCharacter::HandleManaChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{
	if (bAbilitiesInitialized)
	{
		OnManaChanged(DeltaValue, EventTags);
	}
}

void AEscapeCharacter::HandleMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{
	// Update the character movement's walk speed
	GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed();

	if (bAbilitiesInitialized)
	{
		OnMoveSpeedChanged(DeltaValue, EventTags);
	}
}

void AEscapeCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}