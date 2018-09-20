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
	/// ����
	void NotifyConnection(TSharedPtr<FConnection> Connection, void* Data, EErrorCode Error);

	/// �û���¼
	void NotifyUserLogin(TSharedPtr<FConnection> Connection, void* Data, EErrorCode Error);

	/// ƥ����Ϸ
	void NotifyMatchGame(TSharedPtr<FConnection> Connection, void* Data, EErrorCode Error);
	void NotifyCancelGame(TSharedPtr<FConnection> Connection, void* Data, EErrorCode Error);

	/// ����
	void NotifyInvitation(TSharedPtr<FConnection> Connection, void* Data, EErrorCode Error);

	/// ר�÷�����ע��
	void NotifyRegisterServer(TSharedPtr<FConnection> Connection, void* Data, EErrorCode Error);

	/// ��Ϸ״̬
	void NotifyMatchState(TSharedPtr<FConnection> Connection, void* Data, EErrorCode Error);

	/// ��������
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
		
		/// �Ƿ���ע��
		bool bIsRegister;

		/// ����ʱ��
		double StartTime;

		/// ������IP��ַ
		FString IP;

		/// �˿ں�
		int32 Port;
		
		/// ������ΨһID
		FString Guid;

		/// ��С��������
		int32 MinPlayers;

		/// ��Ϸ�������
		int32 MaxPlayers;

		/// ��ǰ�������
		int32 NumPlayers;

		/// ��Ϸ״̬
		EMatchState MatchState;

		/// �߳�ID
		int32 ProcessID;

		/// ��ǰ�߳̾��
		FProcHandle ProcessHandle;
	};

	TArray<TSharedPtr<FDedicatedServerInfo>> DedicatedServerInfoSessions;
};