// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EscapeCharacter.generated.h"


USTRUCT()
struct FAttack
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float Damage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* Montage;
};

UCLASS()
class ESCAPE_API AEscapeCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

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

protected:
	UFUNCTION(Server, WithValidation, Reliable)
	void ServerAttack(uint8 Count);

	UFUNCTION(NetMulticast, Reliable)
	void BroadcastAttack(uint8 Count);
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TArray<FAttack> ComboTable;

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
	FRotator CharacterRotation;

	UPROPERTY()
	FRotator TargetRotation;

	UPROPERTY()
	FRotator LastVelocityRotation;

protected:
	void ManagerCharactorRotation(float DeltaSeconds);

	void SetCharactorRotation(const FRotator& NewTargetRotation, bool bInterp = false, float InterpSpeed = 0.f);

	UFUNCTION(Server, WithValidation, Reliable)
	void ServerSetCharactorRotation(const FRotator& NewTargetRotation, const FRotator& NewCharactorRotation);
};
