// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ImpactActor.generated.h"

USTRUCT()
struct FImpactEffect
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UParticleSystem> ParticleSystem;
};

UCLASS()
class ESCAPE_API AImpactActor : public AActor
{
	GENERATED_UCLASS_BODY()

	virtual void BeginPlay() override;

protected:
	UPROPERTY(Transient)
	TEnumAsByte<EPhysicalSurface> PhysicalSurface;

	UPROPERTY(EditAnywhere, Category = Effect)
	TMap<TEnumAsByte<EPhysicalSurface>, FImpactEffect>  ImpactEffect;
};
