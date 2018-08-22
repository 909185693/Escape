// Copyright 2018 by January. All Rights Reserved.

#include "EscapeCharacter.h"
#include "PostProcessAnimInstance.h"


// Sets default values
AEscapeCharacter::AEscapeCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEscapeCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEscapeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEscapeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);



}

void AEscapeCharacter::Jump()
{
	USkeletalMeshComponent* Mesh = GetMesh();
	UPostProcessAnimInstance* AnimInstance = Mesh ? Cast<UPostProcessAnimInstance>(Mesh->GetAnimInstance()) : nullptr;

	AnimInstance->PlaySlotAnimationAsDynamicMontage(nullptr, NAME_None);
	AnimInstance->NativeUpdateAnimation(0.f);
}
