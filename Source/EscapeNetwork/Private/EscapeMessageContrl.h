// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Object.h"
#include "Misc/Guid.h"
#include "HAL/PlatformProcess.h"
#include "EscapeNetwork.h"
#include "EscapeMessageContrl.generated.h"


UCLASS()
class ESCAPENETWORK_API UEscapeMessageContrl : public UObject
{
	GENERATED_UCLASS_BODY()

	virtual void Register(UEscapeServer* InEscapeServer);
	virtual void BeginDestroy() override;

protected:
	/// ����
	void NotifyConnection(FConnection& Connection, void* Data, EErrorCode Error);

	/// �û���¼
	void NotifyUserLogin(FConnection& Connection, void* Data, EErrorCode Error);

	/// ƥ����Ϸ
	void NotifyMatchGame(FConnection& Connection, void* Data, EErrorCode Error);
	
	/// ����
	void NotifyInvitation(FConnection& Connection, void* Data, EErrorCode Error);

	/// ר�÷�����ע��
	void NotifyRegisterServer(FConnection& Connection, void* Data, EErrorCode Error);

protected:
	UPROPERTY(Transient)
	UEscapeServer* EscapeServer;

	TArray<FConnection> MatchConnections;

	struct FDedicatedServerInfo
	{
		FDedicatedServerInfo()
			: bIsRegister(false)
			, IP(TEXT(""))
			, Port(0)
		{

		}

		bool bIsRegister;

		FString IP;

		int32 Port;

		int32 ConnectionNumber;

		FString Guid;

		int32 ProcessID;

		FProcHandle ProcessHandle;
	};

	TArray<FDedicatedServerInfo> DedicatedServerInfoSessions;
};