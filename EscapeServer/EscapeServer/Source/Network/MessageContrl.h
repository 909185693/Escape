#pragma once

#include "Network/Server.h"

class CServer;

class CMessageContrl : public IDelegateInterface
{
public:
	CMessageContrl(CServer* InServer);
	~CMessageContrl();

private:
	/// 连接
	void NotifyConnection(shared_ptr<FConnection> Connection, void* Data, EErrorCode Error);

	/// 用户登录
	void NotifyUserLogin(shared_ptr<FConnection> Connection, void* Data, EErrorCode Error);

	/// 匹配游戏
	void NotifyMatchGame(shared_ptr<FConnection> Connection, void* Data, EErrorCode Error);
	void NotifyCancelGame(shared_ptr<FConnection> Connection, void* Data, EErrorCode Error);

	/// 专用服务器注册
	void NotifyRegisterServer(shared_ptr<FConnection> Connection, void* Data, EErrorCode Error);

	/// 游戏状态
	void NotifyMatchState(shared_ptr<FConnection> Connection, void* Data, EErrorCode Error);

	/// 房间人数
	void NotifyNumPlayers(shared_ptr<FConnection> Connection, void* Data, EErrorCode Error);

private:
	list<shared_ptr<FConnection>> MatchConnections;

	list<shared_ptr<FConnection>> PendingConnections;
	
	struct FDedicatedServerInfo : public FConnection
	{
		FDedicatedServerInfo(time_t InStartTime, const char* InGuid, HANDLE InProcessHandle)
			: bIsRegister(false)
			, StartTime(InStartTime)
			, Port(0)
			, NumPlayers(0)
			, MatchState(EMatchState::EnteringMap)
			, ProcessHandle(InProcessHandle)
		{
			Socket = NULL;
			NetworkErrorCount = 0;
			strcpy_s(Guid, InGuid);
		}

		void InitConnection(const FConnection& Connection)
		{
			Socket = Connection.Socket;
			NetworkErrorCount = Connection.NetworkErrorCount;
		}

		/// 是否已注册
		bool bIsRegister;

		/// 启动时间
		time_t StartTime;

		/// 服务器IP地址
		char IP[32];

		/// 端口号
		int Port;

		/// 服务器唯一ID
		char Guid[128];

		/// 最小开启人数
		int MinPlayers;

		/// 游戏最大人物
		int MaxPlayers;

		/// 当前玩家数量
		int NumPlayers;

		/// 游戏状态
		EMatchState MatchState;

		/// 当前线程句柄
		HANDLE ProcessHandle;
	};

	list<shared_ptr<FDedicatedServerInfo>> DedicatedServerInfoSessions;

	void ClientTravel();
	bool StartupDedicatedServer();

private:
	CServer* Server;

public:
	void Process();
};