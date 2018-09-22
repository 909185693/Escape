#pragma once

#include <stdio.h>  
#include <winsock2.h>  

#pragma comment(lib,"ws2_32.lib")

class Server
{
public:
	Server();
	~Server();

private:
	SOCKET Socket;

public:
	bool Register();

private:
	void Process();
};
