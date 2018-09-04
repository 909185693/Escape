// Copyright 2018 by January. All Rights Reserved.

#include "EscapeAnimInstance.h"
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


		// 设置角色运动方向
		const FRotator ActorRotator = Owner->GetActorRotation();
		FQuat AQuat = FQuat(Owner->LastVelocityRotation);
		FQuat BQuat = FQuat(ActorRotator.GetInverse());
		const FRotator ComposeRotator = FRotator(BQuat*AQuat);

		Direction = ComposeRotator.Yaw > 180.f ? ComposeRotator.Yaw - 360.f : ComposeRotator.Yaw;

		CardinalDirection = Owner->ConvertDirection(Direction);

		LookOffset = (Owner->GetBaseAimRotation() - Owner->GetActorRotation()).GetNormalized();
	}

	if (CharacterMovement != nullptr)
	{
		MovementMode = CharacterMovement->MovementMode;

		bIsFalling = CharacterMovement->IsFalling();

		bIsAccelerating = CharacterMovement->GetCurrentAcceleration().SizeSquared() > 0.f;
	}
}

