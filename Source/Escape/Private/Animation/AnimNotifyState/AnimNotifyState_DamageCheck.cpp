// Copyright 2018 by January. All Rights Reserved.

#include "AnimNotifyState_DamageCheck.h"
#include "EscapeCharacter.h"


void UAnimNotifyState_DamageCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	AEscapeCharacter* Character = MeshComp ? Cast<AEscapeCharacter>(MeshComp->GetOwner()) : nullptr;
	if (Character != nullptr)
	{
		Character->DamagedClear();
	}
}

void UAnimNotifyState_DamageCheck::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	AEscapeCharacter* Character = MeshComp ? Cast<AEscapeCharacter>(MeshComp->GetOwner()) : nullptr;
	if (Character != nullptr)
	{
		Character->DamageCheck(FrameDeltaTime);
	}
}

void UAnimNotifyState_DamageCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
}

