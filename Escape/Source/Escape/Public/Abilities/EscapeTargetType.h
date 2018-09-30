// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayAbilityTypes.h"
#include "EscapeAbilityTypes.h"
#include "EscapeTargetType.generated.h"

class AActor;
class AEscapeCharacter;
struct FGameplayEventData;

/**
 * Class that is used to determine targeting for abilities
 * It is meant to be blueprinted to run target logic
 * This does not subclass GameplayAbilityTargetActor because this class is never instanced into the world
 * This can be used as a basis for a game-specific targeting blueprint
 * If your targeting is more complicated you may need to instance into the world once or as a pooled actor
 */
UCLASS(Blueprintable, meta = (ShowWorldContextPin))
class ESCAPE_API UEscapeTargetType : public UObject
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	UEscapeTargetType() {}

	/** Called to determine targets to apply gameplay effects to */
	UFUNCTION(BlueprintNativeEvent)
	void GetTargets(AEscapeCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;
};

/** Trivial target type that uses the owner */
UCLASS(NotBlueprintable)
class ESCAPE_API UEscapeTargetType_UseOwner : public UEscapeTargetType
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	UEscapeTargetType_UseOwner() {}

	/** Uses the passed in event data */
	virtual void GetTargets_Implementation(AEscapeCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};

/** Trivial target type that pulls the target out of the event data */
UCLASS(NotBlueprintable)
class ESCAPE_API UEscapeTargetType_UseEventData : public UEscapeTargetType
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	UEscapeTargetType_UseEventData() {}

	/** Uses the passed in event data */
	virtual void GetTargets_Implementation(AEscapeCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};
