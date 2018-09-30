// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "EscapeAbilitySystemComponent.h"
#include "EscapeCharacter.h"
#include "EscapeGameplayAbility.h"
#include "AbilitySystemGlobals.h"

UEscapeAbilitySystemComponent::UEscapeAbilitySystemComponent() {}

void UEscapeAbilitySystemComponent::GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UEscapeGameplayAbility*>& ActiveAbilities)
{
	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, AbilitiesToActivate, false);

	// Iterate the list of all ability specs
	for (FGameplayAbilitySpec* Spec : AbilitiesToActivate)
	{
		// Iterate all instances on this ability spec
		TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

		for (UGameplayAbility* ActiveAbility : AbilityInstances)
		{
			ActiveAbilities.Add(Cast<UEscapeGameplayAbility>(ActiveAbility));
		}
	}
}

int32 UEscapeAbilitySystemComponent::GetDefaultAbilityLevel() const
{
	AEscapeCharacter* OwningCharacter = Cast<AEscapeCharacter>(OwnerActor);

	if (OwningCharacter)
	{
		return OwningCharacter->GetCharacterLevel();
	}
	return 1;
}

UEscapeAbilitySystemComponent* UEscapeAbilitySystemComponent::GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent)
{
	return Cast<UEscapeAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, LookForComponent));
}
