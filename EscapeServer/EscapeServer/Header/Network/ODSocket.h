#ifndef _ODSOCKET_H_
#define _ODSOCKET_H_

#pragma once

#ifdef WIN32
#include<winsock2.h>
#include<WS2tcpip.h>
typedef int socklen_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
typedef int SOCKET;
//#pragma region define win32 const variable in linux
#define INVALID_SOCKET	- 1
#define SOCKET_ERROR	- 1
//#pragma end region
#endif

class ODSocket
{
public:
	ODSocket(SOCKET Socket = INVALID_SOCKET);

	~ODSocket();

	// Create socket object forsnd/recv data
	bool Create(int Af, int Type, int Protocol = 0);

	// Connectsocket
	bool Connect(const char* IP, unsigned short Port);
	
	// #region server
	// Bindsocket
	bool Bind(unsigned short Port);

	// Listensocket
	bool Listen(int Backlog);

	// Accept socket
	bool Accept(ODSocket& Socket, char* FromIP = NULL);
	// #endregion

	// NonBlocking
	bool NonBlocking();

	// Sendsocket
	int Send(const char* Buf, int Len, int Flags = 0);

	// Recvsocket
	int Recv(char* Buf, int Len, int Flags = 0);

	// Closesocket
	int Close();

	// Geterrno
	int GetError();

	//#pragma region just forwin32
	// Init winsock DLL
	static int Init();

	// Clean winsockDLL
	static int Clean();

	// #pragma end region
	// Domain parse
	static bool DnsParse(const char* Domain, char* IP);
	
	ODSocket& operator =(SOCKET Socket);

	operator SOCKET();

protected:
	SOCKET M_Socket;
};
#endif //_ODSOCKET_H_