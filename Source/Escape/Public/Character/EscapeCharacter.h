// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EscapeCharacter.generated.h"


UENUM(BlueprintType)
namespace ECardinalDirection
{
	enum Type
	{
		Forwards,
		Backwards,
		Leftwards,
		Rightwards
	};
}

USTRUCT()
struct FAttack
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float Damage;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere)
	UAnimMontage* Montage;
};

UCLASS()
class ESCAPE_API AEscapeCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void BeginPlay() override;

	/**
	 * Apply damage to this actor.
	 * @see https://www.unrealengine.com/blog/damage-in-ue4
	 * @param DamageAmount		How much damage to apply
	 * @param DamageEvent		Data package that fully describes the damage received.
	 * @param EventInstigator	The Controller responsible for the damage.
	 * @param DamageCauser		The Actor that directly caused the damage (e.g. the projectile that exploded, the rock that landed on you)
	 * @return					The amount of damage actually applied.
	 */
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	/** switch to ragdoll */
	virtual void SetRagdollPhysics();

	/** Broadcast play hit */
	UFUNCTION(NetMulticast, Reliable)
	virtual void PlayHit(float ActualDamage, bool bIsCrit, bool bKilled, APawn* PawnInstigator, AActor* DamageCauser, const FHitResult& HiResult);

	/** Identifies if pawn is in its dying state */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Property")
	uint32 bIsDying : 1;

	UPROPERTY(EditAnywhere, Replicated, Category = Sound)
	class USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, Replicated, Category = Sound)
	class UAnimMontage* DeathMontage;

	/** notification when killed, for both the server and client. */
	virtual void OnDeath(float KillingDamage, class APawn* InstigatingPawn, class AActor* DamageCauser);

	/**
	 *	Function called every frame on this Actor. Override this function to implement custom logic to be executed every frame.
	 *	Note that Tick is disabled by default, and you will need to check PrimaryActorTick.bCanEverTick is set to true to enable it.
	 *
	 *	@param	DeltaSeconds	Game time elapsed during last frame modified by the time dilation
	 */
	virtual void Tick(float DeltaTime) override;

public:
	virtual float GetHealth() const { return Health; };
	virtual float GetMaxHealth() const { return MaxHealth; };
	virtual bool IsAlive() const { return Health > 0.f; };
		
protected:
	UPROPERTY(EditAnywhere, Category = "Property")
	float AttackSpeed;

	UPROPERTY(EditAnywhere, Replicated, Category = "Property")
	float Health;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Property")
	float MaxHealth;

public:
	/** Convert to cardinal direction with direction  */
	virtual TEnumAsByte<ECardinalDirection::Type> ConvertDirection(float NewDirection) const;

	/** Assault Counter */
	UPROPERTY(Transient)
	uint8 AttackCount;

	/** Is save attack */
	UPROPERTY(Transient)
	bool bSaveAttack;

	/** Is attacking */
	UPROPERTY(Transient)
	bool bIsAttacking;

	/**
	 * Called via input to character attack
	 */
	UFUNCTION(BlueprintCallable, Category="Attack")
	virtual void Attack();

	/**
	 * Called via notify character combo attack save
	 */
	UFUNCTION(BlueprintCallable, Category="Attack")
	virtual void ComboAttackSave();

	/**
	 * Called via notify character reset combo
	 */
	UFUNCTION(BlueprintCallable, Category="Attack")
	virtual void ResetCombo();

	/**
	 * Called via notify character damage clear
	 */
	UFUNCTION(BlueprintCallable, Category="Attack")
	virtual void DamagedClear();

	/**
	 * Tick damage chekc when attack state
	 */
	UFUNCTION(BlueprintCallable, Category="Attack")
	virtual void DamageCheck(float DeltaTime);

protected:
	UFUNCTION(Server, WithValidation, Reliable)
	void ServerAttack(uint8 Count);

	UFUNCTION(NetMulticast, Reliable)
	void BroadcastAttack(uint8 Count);
	
	UFUNCTION(NetMulticast, Reliable)
	void PlayDamaged(const FHitResult& HitResult);

	UPROPERTY(EditAnywhere, Category = "Attack")
	TArray<FAttack> ComboTable;

	UPROPERTY(Transient)
	FVector LastWeaponLocation;

	UPROPERTY(Transient)
	TArray<AActor*> DamagedActors;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float DamagedPauseFPSTime;

	UPROPERTY(Transient)
	float LastDamagedPauseFPSTime;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TMap<TEnumAsByte<ECardinalDirection::Type>, UAnimMontage*> GithitMontage;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TMap<TEnumAsByte<EPhysicalSurface>, UParticleSystem*> ImpactParticle;

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* WeaponCapsule;

public:
	UPROPERTY()
	bool bIsMoving;

	UPROPERTY()
	FRotator TargetRotation;

	UPROPERTY()
	FRotator CharacterRotation;

	UPROPERTY()
	FRotator LastVelocityRotation;

	UPROPERTY()
	bool bTurnInPlace;

	UPROPERTY(EditAnywhere, Category = "Animation")
	float TurnInPlaceDelay;

	UPROPERTY(BlueprintReadOnly, Category = "World")
	float TimeSeconds;

protected:
	void ManagerCharactorRotation(float DeltaTime);

	void SetCharactorRotation(const FRotator& NewTargetRotation, bool bInterp = false, float InterpSpeed = 0.f);

	UFUNCTION(Server, WithValidation, Reliable)
	void ServerSetCharactorRotation(const FRotator& NewTargetRotation, const FRotator& NewCharactorRotation);
};
