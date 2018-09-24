#pragma once

#include <stdio.h>
#include <winsock2.h>
#include <memory>
#include <list>
#include <vector>
#include <thread>
#include <mutex>
#include <process.h>
#include "NetworkTypes.h"

#pragma comment(lib,"ws2_32.lib")

using namespace std;

class CMessageContrl;

struct FConnection
{
public:
	FConnection(SOCKET InSocket)
		: Socket(InSocket)
		, NetworkErrorCount(0)
	{

	}

	FConnection()
		: Socket(0)
		, NetworkErrorCount(0)
	{

	}

	bool operator==(const FConnection& Other) const
	{
		return Socket == Other.Socket;
	}

	SOCKET Socket;

	unsigned int NetworkErrorCount;
};

class IDelegateInterface {};

class CServer
{
public:
	CServer();
	~CServer();

private:
	SOCKET Socket;
	shared_ptr<CMessageContrl> CMessageContrlPtr;
	list<shared_ptr<FConnection>> Connections;
	list<shared_ptr<FConnection>> ClosedConnections;

	bool bProcess;
	thread ProcessThread;

protected:
	void Process();

public:
	bool Register();
	bool SendTo(SOCKET SendSocket, ELogicCode Code, EErrorCode Error, int DataSize = 0, void* Data = nullptr);
	bool RecvFrom(SOCKET RecvSocket, void*& OutData, ELogicCode& OutCode, EErrorCode& OutError);
	void CloseConnection(shared_ptr<FConnection> Connection);

protected:
	template<class TClass>
	using FCallbackDelegate = void(TClass::*)(shared_ptr<FConnection>, void*, EErrorCode);

	struct FCallback
	{
		template<class TClass>
		FCallback(ELogicCode InCode, IDelegateInterface* InObject, FCallbackDelegate<TClass>& InFunc)
			: Code(InCode)
			, Object(InObject)
			, Delegate((FDelegate)InFunc)
		{

		}

		ELogicCode Code;

		IDelegateInterface* Object;

		typedef void(IDelegateInterface::*FDelegate)(shared_ptr<FConnection>, void*, EErrorCode);
		
		FDelegate Delegate;
				
		void Execute(shared_ptr<FConnection> Connection, void* Data, EErrorCode Error)
		{
			(Object->*Delegate)(Connection, Data, Error);
		}
	};

	vector<shared_ptr<FCallback>> CallbacksDelegate;

	void ExecuteCallback(shared_ptr<FConnection> Connection, ELogicCode Code, EErrorCode Error, void* Data)
	{
		printf("ExecuteCallback Code[%d] Error[%d] \r\n", Code, Error);

		for (auto It = CallbacksDelegate.begin(); It != CallbacksDelegate.end(); ++It)
		{
			shared_ptr<FCallback> Callback = *It;
			if (Callback->Code == Code)
			{
				Callback->Execute(Connection, Data, Error);
			}
		}
	}

public:
	template <typename TClass>
	inline shared_ptr<FCallback> AddCallback(ELogicCode Code, TClass* InObject, FCallbackDelegate<TClass>&& InFunc)
	{
		shared_ptr<FCallback> Callback = make_shared<FCallback>(Code, InObject, InFunc);
		
		CallbacksDelegate.push_back(Callback);

		return Callback;
	}

	void ClearCallback(void* InObject)
	{
		vector<shared_ptr<FCallback>>::iterator It;
		for (It = CallbacksDelegate.begin(); It != CallbacksDelegate.end();)
		{
			shared_ptr<FCallback> Callback = *It;
			if (Callback->Object == InObject)
			{
				It = CallbacksDelegate.erase(It);
			}
			else
			{
				++It;
			}
		}
	}

	template<typename Struct> bool RegisterConnection(shared_ptr<FConnection> Source, const shared_ptr<Struct> Target)
	{
		Target->InitConnection(*Source);

		for (list<shared_ptr<FConnection>>::iterator It = Connections.begin() ; It != Connections.end(); ++ It)
		{
			if (*It == Source)
			{
				*It = Target;

				return true;
			}
		}

		return false;
	}
};
