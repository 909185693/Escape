// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ObjectMacros.h"
#include "EscapeSystemTypes.generated.h"

#pragma once


USTRUCT(BlueprintType)
struct FEscapeUser
{
	GENERATED_USTRUCT_BODY()

	FEscapeUser()
	{
		FMemory::Memzero(this, sizeof(FEscapeUser));
	}

	UPROPERTY(BlueprintReadOnly)
	int32 ID;

	UPROPERTY(BlueprintReadOnly)
	FString Nickname;

	void Copy(void* Data)
	{
		ID = *((int32*)(Data));
		Nickname = UTF8_TO_TCHAR((char*)((char*)Data + sizeof(int32)));
	}
};