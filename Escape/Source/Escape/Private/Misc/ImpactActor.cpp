// Copyright 2018 by January. All Rights Reserved.

#include "ImpactActor.h"


AImpactActor::AImpactActor(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InitialLifeSpan = 5.f;
}

void AImpactActor::BeginPlay()
{
	Super::BeginPlay();
}