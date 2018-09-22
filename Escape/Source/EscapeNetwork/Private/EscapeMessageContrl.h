// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Object.h"
#include "Misc/Guid.h"
#include "HAL/PlatformProcess.h"
#include "OnlineAsyncTaskManager.h"
#include "EscapeNetwork.h"
#include "EscapeMessageContrl.generated.h"


UCLASS()
class ESCAPENETWORK_API UEscapeMessageContrl : public UObject, public FOnlineAsyncTask
{
	GENERATED_UCLASS_BODY()

	virtual void Register(UEscapeServer* InEscapeServer);
	virtual void BeginDestroy() override;

protected:
	/// 连接
	void NotifyConnection(TSharedPtr<FConnection> Connection, void* Data, EErrorCode Error);

	/// 用户登录
	void NotifyUserLogin(TSharedPtr<FConnection> Connection, void* Data, EErrorCode Error);

	/// 匹配游戏
	void NotifyMatchGame(TSharedPtr<FConnection> Connection, void* Data, EErrorCode Error);
	void NotifyCancelGame(TSharedPtr<FConnection> Connection, void* Data, EErrorCode Error);

	/// 邀请
	void NotifyInvitation(TSharedPtr<FConnection> Connection, void* Data, EErrorCode Error);

	/// 专用服务器注册
	void NotifyRegisterServer(TSharedPtr<FConnection> Connection, void* Data, EErrorCode Error);

	/// 游戏状态
	void NotifyMatchState(TSharedPtr<FConnection> Connection, void* Data, EErrorCode Error);

	/// 房间人数
	void NotifyNumPlayers(TSharedPtr<FConnection> Connection, void* Data, EErrorCode Error);

private:
	void ClientTravel();
	bool StartupDedicatedServer();

protected:
	/// FOnlineAsyncTask
	virtual bool IsDone() const;
	virtual bool WasSuccessful() const;
	virtual void Tick() override;
	virtual FString ToString() const;

protected:
	UPROPERTY(Transient)
	UEscapeServer* EscapeServer;

	TArray<TSharedPtr<FConnection>> MatchConnections;
	TArray<TSharedPtr<FConnection>> PendingMatchConnections;

	struct FDedicatedServerInfo : public FConnection
	{
		FDedicatedServerInfo()
			: bIsRegister(false)
			, IP(TEXT(""))
			, Port(0)
			, NumPlayers(0)
			, MatchState(EMatchState::EnteringMap)
		{
			Socket = nullptr;
			NetworkErrorCount = 0;
		}

		void InitConnection(const FConnection& Connection)
		{
			Socket = Connection.Socket;
			NetworkErrorCount = Connection.NetworkErrorCount;
		}
		
		/// 是否已注册
		bool bIsRegister;

		/// 启动时间
		double StartTime;

		/// 服务器IP地址
		FString IP;

		/// 端口号
		int32 Port;
		
		/// 服务器唯一ID
		FString Guid;

		/// 最小开启人数
		int32 MinPlayers;

		/// 游戏最大人物
		int32 MaxPlayers;

		/// 当前玩家数量
		int32 NumPlayers;

		/// 游戏状态
		EMatchState MatchState;

		/// 线程ID
		int32 ProcessID;

		/// 当前线程句柄
		FProcHandle ProcessHandle;
	};

	TArray<TSharedPtr<FDedicatedServerInfo>> DedicatedServerInfoSessions;
};