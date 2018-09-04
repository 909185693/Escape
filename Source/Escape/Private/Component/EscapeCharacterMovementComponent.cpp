// Copyright 2018 by January. All Rights Reserved.

#include "EscapeCharacterMovementComponent.h"


UEscapeCharacterMovementComponent::UEscapeCharacterMovementComponent(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	JumpZVelocity = 480.f;
	MaxAcceleration = 1024.f;
	AirControl = 0.2f;
	RotationRate = FRotator(0.f, 0.f, 0.f);
	bOrientRotationToMovement = false;
}

