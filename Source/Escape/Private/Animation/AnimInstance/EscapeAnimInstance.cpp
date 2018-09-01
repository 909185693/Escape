// Copyright 2018 by January. All Rights Reserved.

#include "EscapeAnimInstance.h"
#include "Character/EscapeCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


UEscapeAnimInstance::UEscapeAnimInstance(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Speed(0.f)
{

}

void UEscapeAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	class USkeletalMeshComponent* OwnerComponent = GetSkelMeshComponent();
	Owner = OwnerComponent ? Cast<AEscapeCharacter>(OwnerComponent->GetOwner()) : nullptr;
}

void UEscapeAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Owner == nullptr)
	{
		return;
	}

	Speed = Owner->GetVelocity().Size2D();
}

