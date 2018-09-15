// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Object.h"
#include "EscapeNetwork.h"
#include "EscapeMessageContrl.generated.h"


UCLASS()
class ESCAPENETWORK_API UEscapeMessageContrl : public UObject
{
	GENERATED_UCLASS_BODY()

	virtual void Register(UEscapeServer* InEscapeServer);
	virtual void BeginDestroy() override;

protected:
	/// �û���¼
	void NotifyUserLogin(FConnection& Connection, void* Data, EErrorCode Error);

	/// ƥ����Ϸ
	void NotifyMatchGame(FConnection& Connection, void* Data, EErrorCode Error);
	
	/// ����
	void NotifyInvitation(FConnection& Connection, void* Data, EErrorCode Error);

protected:
	UPROPERTY(Transient)
	UEscapeServer* EscapeServer;
};