// Copyright 2018 by January. All Rights Reserved.

#include "EscapeAnimInstance.h"
#include "Character/EscapeCharacter.h"
#include "EscapeCharacterMovementComponent.h"


UEscapeAnimInstance::UEscapeAnimInstance(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Speed(0.f)
	, Direction(0.f)
	, bIsFalling(false)
{

}

void UEscapeAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	class USkeletalMeshComponent* OwnerComponent = GetSkelMeshComponent();
	Owner = OwnerComponent ? Cast<AEscapeCharacter>(OwnerComponent->GetOwner()) : nullptr;
	CharacterMovement = Owner ? Cast<UEscapeCharacterMovementComponent>(Owner->GetCharacterMovement()) : nullptr;
}

void UEscapeAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Owner != nullptr)
	{
		Speed = Owner->GetVelocity().Size2D();

		LookOffset = (Owner->GetBaseAimRotation() - Owner->GetActorRotation()).GetNormalized();
	}

	if (CharacterMovement != nullptr)
	{
		bIsFalling = CharacterMovement->IsFalling();

		bIsAccelerating = CharacterMovement->GetCurrentAcceleration().SizeSquared() > 0.f;
	}
}

