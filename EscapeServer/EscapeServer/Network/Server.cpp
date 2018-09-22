#include "Network/Server.h"

#define BackLog 1024

Server::Server()
{
}

Server::~Server()
{
}

bool Server::Register()
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
		printf("socket error!");

		return false;
	}

	sockaddr_in Addr;
	Addr.sin_family = AF_INET;
	Addr.sin_port = htons(8800);
	Addr.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(Socket, (LPSOCKADDR)&Addr, sizeof(Addr)) == SOCKET_ERROR)
	{
		printf("bind error !");

		return false;
	}

	if (listen(Socket, BackLog) == SOCKET_ERROR)
	{
		printf("listen error !");

		return false;
	}

	return true;
}

void Server::Process()
{

}