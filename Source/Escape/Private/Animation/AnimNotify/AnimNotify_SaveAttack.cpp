// Copyright 2018 by January. All Rights Reserved.

#include "AnimNotify_SaveAttack.h"
#include "EscapeCharacter.h"


void UAnimNotify_SaveAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AEscapeCharacter* Character = MeshComp ? Cast<AEscapeCharacter>(MeshComp->GetOwner()) : nullptr;
	if (Character != nullptr)
	{
		Character->ComboAttackSave();
	}
}


