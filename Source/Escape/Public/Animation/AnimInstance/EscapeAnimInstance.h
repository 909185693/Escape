// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EscapeAnimInstance.generated.h"


class AEscapeCharacter;

/**
 * Escape game character base anim instance
 */
UCLASS()
class ESCAPE_API UEscapeAnimInstance : public UAnimInstance
{
	GENERATED_UCLASS_BODY()

	// Native init override point
	virtual void NativeInitializeAnimation() override;

	// Native update override point
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UPROPERTY(Transient, BlueprintReadOnly)
	class AEscapeCharacter* Owner;

	UPROPERTY(Transient, BlueprintReadOnly)
	class UEscapeCharacterMovementComponent* CharacterMovement;

protected:
	/** ��ɫ�˶��ٶ� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (ClampMin = "0"))
	float Speed;

	/** ��ɫ�˶����� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (ClampMin = "-180", ClampMax = "180"))
	float Direction;
	
	/** ��ɫ�½� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	bool bIsFalling;

	/** ��ɫ���� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	bool bIsAccelerating;

	/** �ӽ�ƫ�� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	FRotator LookOffset;

	/** ��תƫ�� Yaw */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	FRotator RotationOffsetYaw;
};
