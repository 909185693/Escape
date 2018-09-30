#include <stdio.h>
#include "ODSocket.h"

#ifdef WIN32
#pragma comment(lib, "ws2_32.lib")
#endif

ODSocket::ODSocket(SOCKET Socket)
{
	M_Socket = Socket;
}

ODSocket::~ODSocket()
{

}

int ODSocket::Init()
{
#ifdef WIN32
/*
http://msdn.microsoft.com/zh-cn/vstudio/ms741563(en-us,VS.85).aspx
typedef struct WSAData {
        WORDwVersion;								//winsockversion
        WORDwHighVersion;							//Thehighest version of the Windows Sockets specification that the Ws2_32.dll cansupport
        charszDescription[WSADESCRIPTION_LEN+1];
        charszSystemStatus[WSASYSSTATUS_LEN+1];
        unsignedshort iMaxSockets;
        unsignedshort iMaxUdpDg;
        charFAR * lpVendorInfo;
}WSADATA, *LPWSADATA;
*/
	WSADATA WsaData;
	//#define MAKEWORD(a,b) ((WORD)(((BYTE) (a)) | ((WORD) ((BYTE) (b))) << 8))
	WORD Version = MAKEWORD(2, 0);
	int Return = WSAStartup(Version, &WsaData);//win sock startup
	if (Return) {
		//cerr << "Initilize winsock error !" << endl;
		return-1;
	}
#endif

	return 0;
}
//this is just for windows
int ODSocket::Clean()
{
#ifdef WIN32
	return(WSACleanup());
#endif
	return 0;
}

ODSocket& ODSocket::operator = (SOCKET Socket)
{
	M_Socket = Socket;
	return(*this);
}

ODSocket::operator SOCKET()
{
	return M_Socket;
}

//create a socket object win/lin is the same
// af:
bool ODSocket::Create(int Af, int Type, int Protocol)
{
	M_Socket = socket(Af, Type, Protocol);
	if (M_Socket == INVALID_SOCKET) {
		return false;
	}
	return true;
}

bool ODSocket::Connect(const char* IP, unsigned short Port)
{
	struct sockaddr_in SvrAddr;
	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_port = htons(Port);
	inet_pton(AF_INET, IP, (void*)&SvrAddr.sin_addr.s_addr);
	int Return = connect(M_Socket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr));
	if (Return == SOCKET_ERROR) {
		return false;
	}
	return true;
}

bool ODSocket::Bind(unsigned short Port)
{
	struct sockaddr_in SvrAddr;
	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_addr.s_addr = INADDR_ANY;
	SvrAddr.sin_port = htons(Port);
	int opt = 1;
	if (setsockopt(M_Socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0)
		return false;
	int Return = bind(M_Socket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr));
	if (Return == SOCKET_ERROR) {
		return false;
	}
	return true;
}

//for server
bool ODSocket::Listen(int backlog)
{
	int Return = listen(M_Socket, backlog);
	if (Return == SOCKET_ERROR) {
		return false;
	}
	return true;
}

bool ODSocket::Accept(ODSocket& Socket, char* FromIP)
{
	struct sockaddr_in CliAddr;
	socklen_t AddrLen = sizeof(CliAddr);
	SOCKET NewSocket = accept(M_Socket, (struct sockaddr*)&CliAddr, &AddrLen);
	if (NewSocket == SOCKET_ERROR)
	{
		return false;
	}
	Socket = NewSocket;
	if (FromIP != NULL)
	{
		if (inet_ntop(AF_INET, &CliAddr.sin_addr, FromIP, sizeof(FromIP)))
			return true;
	}
	return false;
}

bool ODSocket::NonBlocking()
{
	int iMode = 1;
	if (::ioctlsocket(M_Socket, FIONBIO, (u_long FAR*) &iMode) == SOCKET_ERROR)
	{
		return false;
	}

	return true;
}

int ODSocket::Send(const char* Buf, int Len, int Flags)
{
	int Bytes;
	int Count = 0;
	while (Count < Len) {
		Bytes = send(M_Socket, Buf + Count, Len - Count, Flags);
		if (Bytes == -1 || Bytes == 0)
			return-1;
		Count += Bytes;
	}
	return Count;
}

int ODSocket::Recv(char* Buf, int Len, int Flags)
{
	return (recv(M_Socket, Buf, Len, Flags));
}

int ODSocket::Close()
{
#ifdef WIN32
	return(closesocket(M_Socket));
#else
	return (close(M_Socket));
#endif
}

int ODSocket::GetError()
{
#ifdef WIN32
	return(WSAGetLastError());
#else
	return (errno);
#endif
}

bool ODSocket::DnsParse(const char* Domain, char* IP)
{
	struct addrinfo Hints;
	memset(&Hints, 0, sizeof(struct addrinfo));
	Hints.ai_family = AF_INET;			/* Allow IPv4 */
	Hints.ai_flags = AI_PASSIVE;		/* For wildcard IP address */
	Hints.ai_protocol = 0;				/* Any protocol */
	Hints.ai_socktype = SOCK_STREAM;

	struct addrinfo *Res;
	if (getaddrinfo(Domain, NULL, &Hints, &Res) == NULL)
	{
		for (struct addrinfo* Info = Res; Info != NULL; Info = Info->ai_next) {
			struct sockaddr_in* Addr = (struct sockaddr_in *)Info->ai_addr;
			sprintf_s(IP, sizeof(IP), "%d.%d.%d.%d",
				(*Addr).sin_addr.S_un.S_un_b.s_b1,
				(*Addr).sin_addr.S_un.S_un_b.s_b2,
				(*Addr).sin_addr.S_un.S_un_b.s_b3,
				(*Addr).sin_addr.S_un.S_un_b.s_b4);
		}

		freeaddrinfo(Res);

		return true;
	}

	return true;
}