#include "MessageContrl.h"
#include <assert.h>
#include <WS2tcpip.h>


CMessageContrl::CMessageContrl(CServer* InServer)
	: Server(InServer)
{
	assert(Server);

	//Server->AddToParallelTasks(this);
	Server->AddCallback(LC_USERLOGIN, this, &CMessageContrl::NotifyUserLogin);
	Server->AddCallback(LC_MATCHGAME, this, &CMessageContrl::NotifyMatchGame);
	Server->AddCallback(LC_CANCELMATCH, this, &CMessageContrl::NotifyCancelGame);
	Server->AddCallback(LC_CONNECTION, this, &CMessageContrl::NotifyConnection);
	Server->AddCallback(LC_REGISTERSERVER, this, &CMessageContrl::NotifyRegisterServer);
	Server->AddCallback(LC_MATCHSTATE, this, &CMessageContrl::NotifyMatchState);
	Server->AddCallback(LC_NUMPLAYERS, this, &CMessageContrl::NotifyNumPlayers);
}

CMessageContrl::~CMessageContrl()
{
	for (shared_ptr<FDedicatedServerInfo> DedicatedServerInfo : DedicatedServerInfoSessions)
	{
		::TerminateProcess(DedicatedServerInfo->ProcessHandle, 0);
		::CloseHandle(DedicatedServerInfo->ProcessHandle);
	}
	DedicatedServerInfoSessions.clear();
	PendingConnections.clear();
	MatchConnections.clear();

	Server->ClearCallback(this);
}

void CMessageContrl::NotifyConnection(shared_ptr<FConnection> Connection, void* Data, EErrorCode Error)
{
	if (Error != EC_NONE)
	{
		MatchConnections.remove(Connection);
		PendingConnections.remove(Connection);

		shared_ptr<FDedicatedServerInfo> DedicatedServerInfo = static_pointer_cast<FDedicatedServerInfo>(Connection);
		if (DedicatedServerInfo != nullptr)
		{
			DedicatedServerInfoSessions.remove(DedicatedServerInfo);
		}

		printf("EscapeServer : Disconnect error MatchConnections[%d] DedicatedServerInfoSessions[%d]\r\n",
			(int)MatchConnections.size(),
			(int)DedicatedServerInfoSessions.size());
	}
}

void CMessageContrl::NotifyUserLogin(shared_ptr<FConnection> Connection, void* Data, EErrorCode Error)
{
	FUserLogin* UserLogin = (FUserLogin*)Data;
	if (UserLogin != nullptr)
	{
		if (strcmp(UserLogin->Username, "909185693") == 0 &&
			strcmp(UserLogin->Password, "111111") == 0)
		{
			FUser SendData;
			SendData.ID = 10086;
			strcpy_s(SendData.Nickname, "中文测试Test");
			Server->SendTo(Connection->Socket, LC_USERLOGIN, EC_NONE, sizeof(FUser), &SendData);
		}
		else
		{
			Server->SendTo(Connection->Socket, LC_USERLOGIN, EC_PASSWORDERROR);
		}
	}
}

void CMessageContrl::NotifyMatchGame(shared_ptr<FConnection> Connection, void* Data, EErrorCode Error)
{
	for (shared_ptr<FConnection> TestConnection : MatchConnections)
	{
		if (TestConnection == Connection)
		{
			return;
		}
	}

	MatchConnections.push_back(Connection);
}

void CMessageContrl::NotifyCancelGame(shared_ptr<FConnection> Connection, void* Data, EErrorCode Error)
{
	MatchConnections.remove(Connection);
	PendingConnections.remove(Connection);
}

void CMessageContrl::NotifyRegisterServer(shared_ptr<FConnection> Connection, void* Data, EErrorCode Error)
{
	bool bRegisterSuccess = false;

	FDedicatedServer* DedicatedServer = (FDedicatedServer*)Data;
	if (strcmp(DedicatedServer->Guid, "") == 0)
	{
		time_t CurrentTime = time(NULL);

		shared_ptr<FDedicatedServerInfo> DedicatedServerInfo = make_shared<FDedicatedServerInfo>(CurrentTime, "", nullptr);

		Server->RegisterConnection(Connection, DedicatedServerInfo);

		DedicatedServerInfo->bIsRegister = true;
		DedicatedServerInfo->Port = DedicatedServer->Port;
		DedicatedServerInfo->MinPlayers = DedicatedServer->MinPlayers;
		DedicatedServerInfo->MaxPlayers = DedicatedServer->MaxPlayers;

		SOCKADDR_IN Address;
		memset(&Address, 0, sizeof(Address));
		int AddrLen = sizeof(Address);

		if (::getpeername(DedicatedServerInfo->Socket, (SOCKADDR*)&Address, &AddrLen) == 0)
		{
			char IPdotdec[20];
			inet_ntop(AF_INET, &Address.sin_addr, IPdotdec, 16);

			strcpy_s(DedicatedServerInfo->IP, IPdotdec);

			printf("EscapeServer : Register success dedicated server %s:%d MinPlayers[%d] MaxPlayers[%d]\r\n",
				DedicatedServerInfo->IP,
				DedicatedServerInfo->Port,
				DedicatedServerInfo->MinPlayers,
				DedicatedServerInfo->MaxPlayers);
		}

		bRegisterSuccess = true;

		DedicatedServerInfoSessions.push_back(DedicatedServerInfo);
	}
	else
	{
		for (shared_ptr<FDedicatedServerInfo> DedicatedServerInfo : DedicatedServerInfoSessions)
		{
			if (strcmp(DedicatedServerInfo->Guid, DedicatedServer->Guid) == 0)
			{
				bRegisterSuccess = Server->RegisterConnection(Connection, DedicatedServerInfo);
				if (bRegisterSuccess)
				{
					DedicatedServerInfo->bIsRegister = true;
					DedicatedServerInfo->Port = DedicatedServer->Port;
					DedicatedServerInfo->MinPlayers = DedicatedServer->MinPlayers;
					DedicatedServerInfo->MaxPlayers = DedicatedServer->MaxPlayers;

					SOCKADDR_IN Address;
					memset(&Address, 0, sizeof(Address));
					int AddrLen = sizeof(Address);

					if (::getpeername(DedicatedServerInfo->Socket, (SOCKADDR*)&Address, &AddrLen) == 0)
					{
						char IPdotdec[20];
						inet_ntop(AF_INET, &Address.sin_addr, IPdotdec, 16);

						strcpy_s(DedicatedServerInfo->IP, IPdotdec);

						printf("EscapeServer : Register success dedicated server %s:%d MinPlayers[%d] MaxPlayers[%d]\r\n",
							DedicatedServerInfo->IP,
							DedicatedServerInfo->Port,
							DedicatedServerInfo->MinPlayers,
							DedicatedServerInfo->MaxPlayers);
					}
				}

				break;
			}
		}
	}

	if (!bRegisterSuccess)
	{
		Server->CloseConnection(Connection);
	}
}

void CMessageContrl::NotifyMatchState(shared_ptr<FConnection> Connection, void* Data, EErrorCode Error)
{
	shared_ptr<FDedicatedServerInfo> DedicatedServerInfo = static_pointer_cast<FDedicatedServerInfo>(Connection);
	if (DedicatedServerInfo != nullptr)
	{
		DedicatedServerInfo->MatchState = *(EMatchState*)Data;

		printf("EscapeServer : Sync game state [%d]\r\n", (int)DedicatedServerInfo->MatchState);
	}
}

void CMessageContrl::NotifyNumPlayers(shared_ptr<FConnection> Connection, void* Data, EErrorCode Error)
{
	shared_ptr<FDedicatedServerInfo> DedicatedServerInfo = static_pointer_cast<FDedicatedServerInfo>(Connection);
	if (DedicatedServerInfo != nullptr)
	{
		DedicatedServerInfo->NumPlayers = *(int*)Data;
	}
}

void CMessageContrl::ClientTravel()
{
	const int MinStartupPlayers = 2;
	if (MatchConnections.size() >= MinStartupPlayers)
	{
		StartupDedicatedServer();

		list<shared_ptr<FConnection>>::const_iterator It = MatchConnections.begin();
		for (int Index = 0; Index < MinStartupPlayers && It != MatchConnections.end(); ++Index)
		{
			PendingConnections.push_back(*It);
			It = MatchConnections.erase(It);
		}
	}

	for (shared_ptr<FDedicatedServerInfo> DedicatedServerInfo : DedicatedServerInfoSessions)
	{
		/// 服务器状态
		if (DedicatedServerInfo->MatchState != EMatchState::EnteringMap &&
			DedicatedServerInfo->MatchState != EMatchState::WaitingToStart)
		{
			continue;
		}

		/// 服务器注册超时
		if (!DedicatedServerInfo->bIsRegister)
		{
			time_t CurrentTime = time(NULL);
			if (CurrentTime - DedicatedServerInfo->StartTime > 60)
			{
				Server->CloseConnection(DedicatedServerInfo);
			}
			continue;
		}

		/// 匹配玩家
		if (DedicatedServerInfo->bIsRegister &&
			DedicatedServerInfo->NumPlayers < DedicatedServerInfo->MaxPlayers &&
			(int)(DedicatedServerInfo->NumPlayers + MatchConnections.size() + PendingConnections.size()) >= DedicatedServerInfo->MinPlayers)
		{
			FClientTravel Travel;
			char URL[64];
			sprintf_s(URL, "%s:%d", DedicatedServerInfo->IP, DedicatedServerInfo->Port);
			strcpy_s(Travel.URL, URL);

			list<shared_ptr<FConnection>>::iterator It;
			for (It = PendingConnections.begin(); It != PendingConnections.end();)
			{
				shared_ptr<FConnection> ClientConnection = *It;

				PendingConnections.erase(It++);

				Server->SendTo(ClientConnection->Socket, LC_CLIENTTRAVEL, EC_NONE, sizeof(FClientTravel), &Travel);

				if (++DedicatedServerInfo->NumPlayers >= DedicatedServerInfo->MaxPlayers)
				{
					break;
				}
			}

			for (It = MatchConnections.begin(); It != MatchConnections.end();)
			{
				shared_ptr<FConnection> ClientConnection = *It;

				MatchConnections.erase(It++);

				Server->SendTo(ClientConnection->Socket, LC_CLIENTTRAVEL, EC_NONE, sizeof(FClientTravel), &Travel);

				if (++DedicatedServerInfo->NumPlayers >= DedicatedServerInfo->MaxPlayers)
				{
					break;
				}
			}
		}
	}
}

bool CMessageContrl::StartupDedicatedServer()
{
	const char* MapName = "EscapeMap";
	const char* GameMode = "Game";

	GUID NewGuid;
	if (!CoCreateGuid(&NewGuid))
	{
		char Guid[64];

		_snprintf_s(Guid, sizeof(Guid),
			"%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X",
			NewGuid.Data1, NewGuid.Data2, NewGuid.Data3,
			NewGuid.Data4[0], NewGuid.Data4[1], NewGuid.Data4[2],
			NewGuid.Data4[3], NewGuid.Data4[4], NewGuid.Data4[5],
			NewGuid.Data4[6], NewGuid.Data4[7]);

		// initialize process attributes
		SECURITY_ATTRIBUTES Attr;
		Attr.nLength = sizeof(SECURITY_ATTRIBUTES);
		Attr.lpSecurityDescriptor = NULL;
		Attr.bInheritHandle = true;

		// initialize window flags
		unsigned int dwFlags = 0;
		unsigned short int ShowWindowFlags = SW_HIDE;
		if (true)
		{
			dwFlags = STARTF_USESHOWWINDOW;
		}
		else if (true)
		{
			dwFlags = STARTF_USESHOWWINDOW;
			ShowWindowFlags = SW_SHOWMINNOACTIVE;
		}

		// initialize process creation flags
		unsigned int CreateFlags = NORMAL_PRIORITY_CLASS | DETACHED_PROCESS;

		// initialize startup info
		STARTUPINFO StartupInfo = {
			sizeof(STARTUPINFO),
			NULL, NULL, NULL,
			(::DWORD)CW_USEDEFAULT,
			(::DWORD)CW_USEDEFAULT,
			(::DWORD)CW_USEDEFAULT,
			(::DWORD)CW_USEDEFAULT,
			(::DWORD)0, (::DWORD)0, (::DWORD)0,
			(::DWORD)dwFlags,
			ShowWindowFlags,
			0, NULL,
			HANDLE(nullptr),
			HANDLE(nullptr),
			HANDLE(nullptr)
		};

		PROCESS_INFORMATION ProcessInfo;

		char ServerDir[] = "..\\..\\WindowsServer\\EscapeServer.exe";
		char CommandLine[256];
		sprintf_s(CommandLine, "\"%s\" %s?game=%s?guid=%s", ServerDir, MapName, GameMode, Guid);

		if (::CreateProcess(NULL, CommandLine, &Attr, &Attr, true, (::DWORD)CreateFlags, NULL, NULL, &StartupInfo, &ProcessInfo))
		{
			time_t StartTime = time(NULL);

			shared_ptr<FDedicatedServerInfo> NewSession = make_shared<FDedicatedServerInfo>(StartTime, Guid, ProcessInfo.hProcess);

			DedicatedServerInfoSessions.push_back(NewSession);

			printf_s("Run a dedicated server of the game. Guid[%s] MapName[%s] GameMode[%s] StartTime[%f]\r\n",
				NewSession->Guid,
				MapName,
				GameMode,
				(double)StartTime);

			return true;
		}
	}

	printf_s("Failed to run a dedicated server of the game.\r\n");

	return false;
}

void CMessageContrl::Process()
{
	ClientTravel();
}