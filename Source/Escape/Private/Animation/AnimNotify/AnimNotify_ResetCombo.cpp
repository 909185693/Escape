// Copyright 2018 by January. All Rights Reserved.

#include "AnimNotify_ResetCombo.h"
#include "EscapeCharacter.h"


void UAnimNotify_ResetCombo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AEscapeCharacter* Character = MeshComp ? Cast<AEscapeCharacter>(MeshComp->GetOwner()) : nullptr;
	if (Character != nullptr)
	{
		Character->ResetCombo();
	}
}
