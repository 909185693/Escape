// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "EscapeTypes.h"
#include "EscapeCharacter.generated.h"


class AEscapeWeapon;
class UEscapeAttributeSet;
class UEscapeAbilitySystemComponent;

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
class ESCAPE_API AEscapeCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_UCLASS_BODY()

protected:
	/** Allow actors to initialize themselves on the C++ side */
	virtual void PostInitializeComponents() override;

	/** Overridable native event for when play begins for this actor. */
	virtual void BeginPlay() override;

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
	class UAnimationAsset* DeathAnimationAsset;

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
	class AEscapeWeapon* GetWeapon() const { return Weapon; }
	class UCapsuleComponent* GetWeaponCollision() const;	// Implement IAbilitySystemInterface
	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(Transient, ReplicatedUsing = OnRep_Weapon)
	class AEscapeWeapon* Weapon;

	void EquipWeapon(TSoftClassPtr<AEscapeWeapon> NewWeaponClass);

	void EquipWeapon(AEscapeWeapon* NewWeapon, AEscapeWeapon* LastWeapon = nullptr);

	UFUNCTION()
	void OnRep_Weapon(AEscapeWeapon* LastWeapon);

	UPROPERTY(EditAnywhere, Replicated, Category = "Property")
	TSoftClassPtr<AEscapeWeapon> WeaponClass;

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

public:
	void LaunchCharacter();

	/**
	 * Set a pending launch velocity on the Character. This velocity will be processed on the next CharacterMovementComponent tick,
	 * and will set it to the "falling" state. Triggers the OnLaunched event.
	 * @PARAM LaunchVelocity is the velocity to impart to the Character
	 * @PARAM bXYOverride if true replace the XY part of the Character's velocity instead of adding to it.
	 * @PARAM bZOverride if true replace the Z component of the Character's velocity instead of adding to it.
	 */
	void LaunchCharacter(FVector LaunchVelocity, bool bXYOverride, bool bZOverride);

protected:
	UPROPERTY(EditAnywhere, Category = Animation)
	float LaunchSpeed;

	UPROPERTY(EditAnywhere, Category = Animation)
	class UAnimMontage* LaunchMontage;

	UFUNCTION(Server, WithValidation, Reliable)
	void ServerLaunchCharacter(FVector LaunchVelocity, bool bXYOverride, bool bZOverride);
	
	UFUNCTION(NetMulticast, Reliable)
	void BroadcastLaunchCharacter(FVector LaunchVelocity, bool bXYOverride, bool bZOverride);

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

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

	/// Abilities

	/** List of attributes modified by the ability system */
	UPROPERTY()
	UEscapeAttributeSet* AttributeSet;

	/** The component used to handle ability system interactions */
	UPROPERTY()
	UEscapeAbilitySystemComponent* AbilitySystemComponent;

	/** If true we have initialized our abilities */
	UPROPERTY()
	int32 bAbilitiesInitialized;

	/** Map of slot to ability granted by that slot. I may refactor this later */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TMap<FEscapeItemSlot, FGameplayAbilitySpecHandle> SlottedAbilities;

public:
	/** Returns current pawn is alive */
	UFUNCTION(BlueprintCallable)
	virtual bool IsAlive() const;

	/** Returns current health, will be 0 if dead */
	UFUNCTION(BlueprintCallable)
	virtual float GetHealth() const;

	/** Returns maximum health, health will never be greater than this */
	UFUNCTION(BlueprintCallable)
	virtual float GetMaxHealth() const;

	/** Returns current mana */
	UFUNCTION(BlueprintCallable)
	virtual float GetMana() const;

	/** Returns maximum mana, mana will never be greater than this */
	UFUNCTION(BlueprintCallable)
	virtual float GetMaxMana() const;

	/** Returns current movement speed */
	UFUNCTION(BlueprintCallable)
	virtual float GetMoveSpeed() const;

	/** Returns current movement speed */
	UFUNCTION(BlueprintCallable)
	virtual float GetAttackSpeed() const;

	/** Returns the character level that is passed to the ability system */
	UFUNCTION(BlueprintCallable)
	virtual int32 GetCharacterLevel() const;

	/** Modifies the character level, this may change abilities. Returns true on success */
	UFUNCTION(BlueprintCallable)
	virtual bool SetCharacterLevel(int32 NewLevel);

	/**
	 * Attempts to activate any ability in the specified item slot. Will return false if no activatable ability found or activation fails
	 * Returns true if it thinks it activated, but it may return false positives due to failure later in activation.
	 * If bAllowRemoteActivation is true, it will remotely activate local/server abilities, if false it will only try to locally activate the ability
	 */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool ActivateAbilitiesWithItemSlot(FEscapeItemSlot ItemSlot, bool bAllowRemoteActivation = true);

	/** Returns a list of active abilities bound to the item slot. This only returns if the ability is currently running */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void GetActiveAbilitiesWithItemSlot(FEscapeItemSlot ItemSlot, TArray<UEscapeGameplayAbility*>& ActiveAbilities);

	/**
	 * Attempts to activate all abilities that match the specified tags
	 * Returns true if it thinks it activated, but it may return false positives due to failure later in activation.
	 * If bAllowRemoteActivation is true, it will remotely activate local/server abilities, if false it will only try to locally activate the ability
	 */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation = true);

	/** Returns a list of active abilities matching the specified tags. This only returns if the ability is currently running */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UEscapeGameplayAbility*>& ActiveAbilities);

	/** Returns total time and remaining time for cooldown tags. Returns false if no active cooldowns found */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration);

	// Called from EscapeAttributeSet, these call BP events above
	virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, AEscapeCharacter* InstigatorCharacter, AActor* DamageCauser);
	virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleManaChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	/**
	 * Called when character takes damage, which may have killed them
	 *
	 * @param DamageAmount Amount of damage that was done, not clamped based on current health
	 * @param HitInfo The hit info that generated this damage
	 * @param DamageTags The gameplay tags of the event that did the damage
	 * @param InstigatorCharacter The character that initiated this damage
	 * @param DamageCauser The actual actor that did the damage, might be a weapon or projectile
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnDamaged(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, AEscapeCharacter* InstigatorCharacter, AActor* DamageCauser);

	/**
	 * Called when health is changed, either from healing or from being damaged
	 * For damage this is called in addition to OnDamaged/OnKilled
	 *
	 * @param DeltaValue Change in health value, positive for heal, negative for cost. If 0 the delta is unknown
	 * @param EventTags The gameplay tags of the event that changed mana
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	/**
	 * Called when mana is changed, either from healing or from being used as a cost
	 *
	 * @param DeltaValue Change in mana value, positive for heal, negative for cost. If 0 the delta is unknown
	 * @param EventTags The gameplay tags of the event that changed mana
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnManaChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	/**
	 * Called when movement speed is changed
	 *
	 * @param DeltaValue Change in move speed
	 * @param EventTags The gameplay tags of the event that changed mana
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
};
