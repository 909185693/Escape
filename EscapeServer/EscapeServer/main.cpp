#include <iostream>
#include <stdio.h>
#include "Network/Server.h"

using namespace std;

int main(int argc, char* argv[])
{
	bool bRegister = false;

	Server* EscapeServer = new Server();
	if (EscapeServer != nullptr)
	{
		bRegister = EscapeServer->Register();
	}

	char Command[256];
	while (bRegister)
	{
		cin >> Command;
	}

	return 0;
}