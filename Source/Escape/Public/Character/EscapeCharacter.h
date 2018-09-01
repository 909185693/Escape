// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EscapeCharacter.generated.h"


UCLASS()
class ESCAPE_API AEscapeCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/** Assault Counter */
	UPROPERTY(Transient)
	uint8 AttackCount;

	/**
	* Called via input to character attack
	*/
	UFUNCTION(BlueprintCallable, Category="EscapeCharacter")
	virtual void Attack();

	/**
	* Called via input to character stop attacking
	*/
	UFUNCTION(BlueprintCallable, Category="EscapeCharacter")
	virtual void StopAttacking();

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
};
