#include "Network/Server.h"
#include "MessageContrl.h"

#define MaxNetworkErrorCount 20

CServer::CServer()
	: Socket(0)
{

}

CServer::~CServer()
{
	bProcess = false;

	closesocket(Socket);

	/// 关闭连接
	for (shared_ptr<FConnection> Connection : Connections)
	{
		closesocket(Connection->Socket);
		Connection->Socket = NULL;
	}

	for (shared_ptr<FConnection> Connection : ClosedConnections)
	{
		auto It = Connections.begin();
		while (It != Connections.end())
		{
			if (Connection == *It)
			{
				closesocket(Connection->Socket);
				Connection->Socket = NULL;

				It = Connections.erase(It);
			}
			else
			{
				++It;
			}
		}
	}

	CMessageContrlPtr.reset();

	WSACleanup();
}

void CServer::Process()
{
	bProcess = true;

	while (bProcess)
	{
		/// accept
		sockaddr_in ClntAddr;
		int nSize = sizeof(SOCKADDR);
		SOCKET AcceptSocket = accept(Socket, (SOCKADDR*)&ClntAddr, &nSize);
		if (AcceptSocket != SOCKET_ERROR)
		{
			shared_ptr<FConnection> NewConnection = make_shared<FConnection>(AcceptSocket);

			Connections.push_back(NewConnection);
		}

		/// 接收消息
		for (shared_ptr<FConnection> Connection : Connections)
		{
			void* Data = nullptr;

			ELogicCode Code = ELogicCode::INVALID;
			EErrorCode Error = EErrorCode::NONE;

			if (RecvFrom(Connection->Socket, Data, Code, Error))
			{
				bool bSeriousError = false;
				if (Error == INVALID_DATA)
				{
					bSeriousError = true;
				}
				else if (Error == NETWORK_ERROR)
				{
					if (++Connection->NetworkErrorCount > MaxNetworkErrorCount)
					{
						bSeriousError = true;
					}
				}
				else
				{
					Connection->NetworkErrorCount = 0;

					ExecuteCallback(Connection, Code, Error, Data);

					free(Data);
				}

				if (bSeriousError)
				{
					CloseConnection(Connection);
				}
			}
		}

		/// 关闭连接
		for (shared_ptr<FConnection> Connection : ClosedConnections)
		{
			auto It = Connections.begin();
			while (It != Connections.end())
			{
				if (Connection == *It)
				{
					ExecuteCallback(Connection, ELogicCode::CONNECTION, EErrorCode::NETWORK_ERROR, nullptr);

					closesocket(Connection->Socket);
					Connection->Socket = NULL;

					It = Connections.erase(It);
				}
				else
				{
					++It;
				}
			}
		}

		ClosedConnections.clear();

		if (CMessageContrlPtr != nullptr)
		{
			CMessageContrlPtr->Process();
		}
	}
}

bool CServer::Register()
{
	WORD SockVersion = MAKEWORD(2, 2);
	WSADATA WsaData;
	if (WSAStartup(SockVersion, &WsaData) != 0)
	{
		return 0;
	}

	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Socket == INVALID_SOCKET)
	{
		printf("socket error!\r\n");

		return false;
	}

	sockaddr_in Addr;
	Addr.sin_family = AF_INET;
	Addr.sin_port = htons(8800);
	Addr.sin_addr.S_un.S_addr = INADDR_ANY;
	if (::bind(Socket, (LPSOCKADDR)&Addr, sizeof(Addr)) == SOCKET_ERROR)
	{
		printf("bind error !\r\n");

		return false;
	}

	if (::listen(Socket, 0) == SOCKET_ERROR)
	{
		printf("listen error !\r\n");

		return false;
	}

	int iMode = 1;
	if (::ioctlsocket(Socket, FIONBIO, (u_long FAR*) &iMode) == SOCKET_ERROR)
	{
		printf("ioctlsocket failed!\r\n");

		return false;
	}

	CMessageContrlPtr = make_shared<CMessageContrl>(this);

	ProcessThread = thread(&CServer::Process, this);
	ProcessThread.detach();

	printf("Startup server.\r\n");

	return true;
}

bool CServer::SendTo(SOCKET SendSocket, ELogicCode Code, EErrorCode Error, int DataSize, void* Data)
{
	int Count = DataSize + sizeof(FDataHeader);
	void* SendData = malloc(Count);

	FDataHeader* DataHeader = (FDataHeader*)SendData;
	DataHeader->Init(Code, Error, DataSize);

	if (DataSize > 0)
	{
		memcpy((void*)(DataHeader + 1), Data, DataSize);
	}

	int BytesSent = send(SendSocket, (char*)SendData, Count, NULL);

	free(SendData);

	if (BytesSent == SOCKET_ERROR)
	{
		return false;
	}
	else
	{
		return true;
	}
}

struct timeval Timeout = { 0, 200 };
struct fd_set Rfds;
bool CServer::RecvFrom(SOCKET RecvSocket, void*& OutData, ELogicCode& OutCode, EErrorCode& OutError)
{
	OutCode = ELogicCode::INVALID;
	OutError = EErrorCode::NONE;

	FD_ZERO(&Rfds);
	FD_SET(RecvSocket, &Rfds);

	switch (select(0, &Rfds, NULL, NULL, &Timeout))
	{
	case SOCKET_ERROR:
		OutError = EErrorCode::NETWORK_ERROR;
		return true;
	case 0:
		break;
	default:
		if (FD_ISSET(RecvSocket, &Rfds))
		{
			const int HeaderSize = sizeof(FDataHeader);
			char HeaderData[HeaderSize];

			int BytesRead = recv(RecvSocket, HeaderData, HeaderSize, NULL);
			if (BytesRead != SOCKET_ERROR)
			{
				FDataHeader* DataHander = (FDataHeader*)HeaderData;

				if (HeaderSize != BytesRead || !DataHander->IsValid())
				{
					OutError = EErrorCode::INVALID_DATA;

					return true;
				}

				OutCode = DataHander->Code;
				OutError = DataHander->Error;

				if (DataHander->Size > 0)
				{
					void* RecvData = malloc(DataHander->Size);

					while (BytesRead > 0)
					{
						BytesRead = recv(RecvSocket, (char*)RecvData, DataHander->Size, NULL);
						if (BytesRead == SOCKET_ERROR)
						{
							OutError = EErrorCode::NETWORK_ERROR;

							return true;
						}

						if (BytesRead == 0 || BytesRead == DataHander->Size)
						{
							OutData = RecvData;

							return true;
						}
					}

					OutError = EErrorCode::NETWORK_ERROR;
				}

				return true;
			}
			else
			{
				OutError = EErrorCode::NETWORK_ERROR;

				return true;
			}
		}
	}

	return false;
}

void CServer::CloseConnection(shared_ptr<FConnection> Connection)
{
	ClosedConnections.push_back(Connection);
}
