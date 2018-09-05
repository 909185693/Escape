// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/EscapeCharacter.h"
#include "EscapeCharacter_Player.generated.h"


/**
 * Íæ¼Ò¿ØÖÆ
 */
UCLASS()
class ESCAPE_API AEscapeCharacter_Player : public AEscapeCharacter
{
	GENERATED_UCLASS_BODY()
	
	/**
	 *	Function called every frame on this Actor. Override this function to implement custom logic to be executed every frame.
	 *	Note that Tick is disabled by default, and you will need to check PrimaryActorTick.bCanEverTick is set to true to enable it.
	 *
	 *	@param	DeltaSeconds	Game time elapsed during last frame modified by the time dilation
	 */
	virtual void Tick(float DeltaTime) override;

	/** If true, adds default input bindings for movement and camera look. */
	UPROPERTY(Category = "Player", EditAnywhere, BlueprintReadOnly)
	uint32 bAddDefaultMovementBindings : 1;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	 * Input callback to move forward in local space (or backward if Val is negative).
	 * @param Val Amount of movement in the forward direction (or backward if negative).
	 * @see APawn::AddMovementInput()
	 */
	UFUNCTION(BlueprintCallable, Category = "Player")
	virtual void MoveForward(float Val);

	/**
	 * Input callback to strafe right in local space (or left if Val is negative).
	 * @param Val Amount of movement in the right direction (or left if negative).
	 * @see APawn::AddMovementInput()
	 */
	UFUNCTION(BlueprintCallable, Category = "Player")
	virtual void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	UFUNCTION(BlueprintCallable, Category="Player")
	virtual void TurnAtRate(float Rate);

	/**
	 * Called via input to look up at a given rate (or down if Rate is negative).
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	UFUNCTION(BlueprintCallable, Category="Player")
	virtual void LookUpAtRate(float Rate);
	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player")
	float BaseTurnRate;

	/** Base lookup rate, in deg/sec. Other scaling may affect final lookup rate. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player")
	float BaseLookUpRate;

	/** Is attack pressed */
	UPROPERTY(Transient)
	bool bAttackPressed;

	/**
	 * Called via input to character attack
	 */
	UFUNCTION(BlueprintCallable, Category = "Attack")
	virtual void StartAttack();

	/**
	 * Called via input to character stop attacking
	 */
	UFUNCTION(BlueprintCallable, Category = "Attack")
	virtual void StopAttacking();
	
};
