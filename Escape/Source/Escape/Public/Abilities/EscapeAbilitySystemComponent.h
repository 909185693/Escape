// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "Abilities/EscapeAbilityTypes.h"
#include "EscapeAbilitySystemComponent.generated.h"

class UEscapeGameplayAbility;

/**
 * Subclass of ability system component with game-specific data
 * Most games will need to make a game-specific subclass to provide utility functions
 */
UCLASS()
class ESCAPE_API UEscapeAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Constructors and overrides
	UEscapeAbilitySystemComponent();

	/** Returns a list of currently active ability instances that match the tags */
	void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UEscapeGameplayAbility*>& ActiveAbilities);

	/** Returns the default level used for ability activations, derived from the character */
	int32 GetDefaultAbilityLevel() const;

	/** Version of function in AbilitySystemGlobals that returns correct type */
	static UEscapeAbilitySystemComponent* GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent = false);

};