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

	Socket.Close();

	/// 关闭连接
	for (shared_ptr<FConnection> Connection : Connections)
	{
		Connection->Socket.Close();
		Connection->Socket = NULL;
	}

	for (shared_ptr<FConnection> Connection : ClosedConnections)
	{
		auto It = Connections.begin();
		while (It != Connections.end())
		{
			if (Connection == *It)
			{
				Connection->Socket.Close();
				Connection->Socket = NULL;

				It = Connections.erase(It);
			}
			else
			{
				++It;
			}
		}
	}

	ODSocket::Clean();

	CMessageContrlPtr.reset();
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

			ELogicCode Code = LC_INVALID;
			EErrorCode Error = EC_NONE;

			if (RecvFrom(Connection->Socket, Data, Code, Error))
			{
				bool bSeriousError = false;
				if (Error == EC_INVALIDDATA)
				{
					bSeriousError = true;
				}
				else if (Error == EC_NETWORKERROR)
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
					ExecuteCallback(Connection, LC_CONNECTION, EC_NETWORKERROR, nullptr);

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
	ODSocket::Init();
	
	if (!Socket.Create(AF_INET, SOCK_STREAM, 0))
	{
		printf("Socket create error!\r\n");

		return false;
	}

	if (!Socket.Bind(8800))
	{
		printf("Socket bind error !\r\n");

		return false;
	}

	if (!Socket.Listen(0))
	{
		printf("Socket listen error !\r\n");

		return false;
	}
	
	if (!Socket.NonBlocking())
	{
		printf("Socket non blocking !\r\n");

		return false;
	}

	CMessageContrlPtr = make_shared<CMessageContrl>(this);

	ProcessThread = thread(&CServer::Process, this);
	ProcessThread.detach();

	printf("Startup server.\r\n");

	char IP[20];
	if (ODSocket::DnsParse("http:://www.baidu.com", IP))
	{
		printf_s("IP : %s", IP);
	}

	return true;
}

bool CServer::SendTo(ODSocket SendSocket, ELogicCode Code, EErrorCode Error, int DataSize, void* Data)
{
	int Count = DataSize + sizeof(FDataHeader);
	void* SendData = malloc(Count);

	FDataHeader* DataHeader = (FDataHeader*)SendData;
	DataHeader->Init(Code, Error, DataSize);

	if (DataSize > 0)
	{
		memcpy((void*)(DataHeader + 1), Data, DataSize);
	}

	int BytesSent = Socket.Send((char*)SendData, Count);

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
bool CServer::RecvFrom(ODSocket RecvSocket, void*& OutData, ELogicCode& OutCode, EErrorCode& OutError)
{
	OutCode = LC_INVALID;
	OutError = EC_NONE;

	FD_ZERO(&Rfds);
	FD_SET(RecvSocket, &Rfds);

	switch (select(0, &Rfds, NULL, NULL, &Timeout))
	{
	case SOCKET_ERROR:
		OutError = EC_NETWORKERROR;
		return true;
	case 0:
		break;
	default:
		if (FD_ISSET(RecvSocket, &Rfds))
		{
			const int HeaderSize = sizeof(FDataHeader);
			char HeaderData[HeaderSize];

			int BytesRead = RecvSocket.Recv(HeaderData, HeaderSize, NULL);
			if (BytesRead != SOCKET_ERROR)
			{
				FDataHeader* DataHander = (FDataHeader*)HeaderData;

				if (HeaderSize != BytesRead || !DataHander->IsValid())
				{
					OutError = EC_INVALIDDATA;

					return true;
				}

				OutCode = DataHander->Code;
				OutError = DataHander->Error;

				if (DataHander->Size > 0)
				{
					void* RecvData = malloc(DataHander->Size);

					while (BytesRead > 0)
					{
						BytesRead = RecvSocket.Recv((char*)RecvData, DataHander->Size, NULL);
						if (BytesRead == SOCKET_ERROR)
						{
							OutError = EC_NETWORKERROR;

							return true;
						}

						if (BytesRead == 0 || BytesRead == DataHander->Size)
						{
							OutData = RecvData;

							return true;
						}
					}

					OutError = EC_NETWORKERROR;
				}

				return true;
			}
			else
			{
				OutError = EC_NETWORKERROR;

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
