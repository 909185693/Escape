// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Object.h"
#include "EscapeMessageContrl.generated.h"


class UEscapeServer;

UCLASS()
class ESCAPENETWORK_API UEscapeMessageContrl : public UObject
{
	GENERATED_UCLASS_BODY()

	virtual void Register(UEscapeServer* InEscapeServer);
	virtual void BeginDestroy() override;

protected:

protected:
	UPROPERTY()
	UEscapeServer* EscapeServer;
};