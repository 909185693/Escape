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
	/// 连接
	void NotifyConnection(FConnection& Connection, void* Data, EErrorCode Error);

	/// 用户登录
	void NotifyUserLogin(FConnection& Connection, void* Data, EErrorCode Error);

	/// 匹配游戏
	void NotifyMatchGame(FConnection& Connection, void* Data, EErrorCode Error);
	
	/// 邀请
	void NotifyInvitation(FConnection& Connection, void* Data, EErrorCode Error);

	/// 专用服务器注册
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