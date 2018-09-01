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
	AEscapeCharacter* Owner;

protected:
	/** 角色运动速度 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (ClampMin = "0"))
	float Speed;

	/** 角色运动方向 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (ClampMin = "-180", ClampMax = "180"))
	float Direction;
	
	
};
