#include <iostream>
#include <stdio.h>
#include "Network/Server.h"

using namespace std;

int main(int argc, char* argv[])
{
	bool bRegister = false;

	CServer* Server = new CServer();
	if (Server != nullptr)
	{
		bRegister = Server->Register();
	}

	char Command[256];
	while (true)
	{
		if (strcmp(Command, "exit") == 0)
		{
			delete Server;
			Server = nullptr;
			break;
		}

		cin >> Command;
	}

	return 0;
}