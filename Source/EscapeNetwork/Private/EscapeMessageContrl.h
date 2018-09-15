// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Object.h"
#include "EscapeNetWorkTypes.h"
#include "EscapeMessageContrl.generated.h"


class UEscapeServer;

UCLASS()
class ESCAPENETWORK_API UEscapeMessageContrl : public UObject
{
	GENERATED_UCLASS_BODY()

	virtual void Register(UEscapeServer* InEscapeServer);
	virtual void BeginDestroy() override;

protected:
	/// ”√ªßµ«¬º
	void NotifyUserLogin(FSocket* Socket, void* Data, EErrorCode Error);

	/// ∆•≈‰”Œœ∑
	void NotifyMatchGame(FSocket* Socket, void* Data, EErrorCode Error);
	
	/// —˚«Î
	void NotifyInvitation(FSocket* Socket, void* Data, EErrorCode Error);

protected:
	UPROPERTY()
	UEscapeServer* EscapeServer;
};