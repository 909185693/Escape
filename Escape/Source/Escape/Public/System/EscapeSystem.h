// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Object.h"
#include "EscapeNetwork.h"
#include "EscapeSystemTypes.h"
#include "EscapeSystem.generated.h"


class UEscapeClient;

UCLASS(Transient, config = Engine)
class UEscapeSystem : public UObject
{
	GENERATED_UCLASS_BODY()

	void Register(UEscapeClient* EscapeClient);

	TSharedPtr<FEscapeUser> GetEscapeUser() const;

protected:
	TSharedPtr<FEscapeUser> EscapeUser;

protected:
	void UpdateEscapeUser(void* Data, EErrorCode Error);
};