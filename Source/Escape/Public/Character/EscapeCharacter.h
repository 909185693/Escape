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

	UFUNCTION(NetMulticast, Reliable)
	virtual void PlayHit(float DamageAmount, APawn* InstigatorPawn, AActor* DamageCauser, const FHitResult& HiResult);

	/**
	 *	Function called every frame on this Actor. Override this function to implement custom logic to be executed every frame.
	 *	Note that Tick is disabled by default, and you will need to check PrimaryActorTick.bCanEverTick is set to true to enable it.
	 *
	 *	@param	DeltaSeconds	Game time elapsed during last frame modified by the time dilation
	 */
	virtual void Tick(float DeltaTime) override;

public:
	/** Convert to cardinal direction with direction  */
	virtual TEnumAsByte<ECardinalDirection::Type> ConvertDirection(float NewDirection) const;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackSpeed;

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
	* Called via input to character stop attacking
	*/
	UFUNCTION(BlueprintCallable, Category="Attack")
	virtual void StopAttacking();

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
	
	UFUNCTION()
	void PlayImpactEffect(const FHitResult& HitResult);

	UPROPERTY(EditAnywhere, Category = "Attack")
	TArray<FAttack> ComboTable;

	UPROPERTY(Transient)
	FVector LastWeaponLocation;

	UPROPERTY(Transient)
	TArray<AActor*> DamagedActors;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float TimeDilationInterpSpeed;

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

protected:
	void ManagerCharactorRotation(float DeltaTime);

	void SetCharactorRotation(const FRotator& NewTargetRotation, bool bInterp = false, float InterpSpeed = 0.f);

	UFUNCTION(Server, WithValidation, Reliable)
	void ServerSetCharactorRotation(const FRotator& NewTargetRotation, const FRotator& NewCharactorRotation);
};
