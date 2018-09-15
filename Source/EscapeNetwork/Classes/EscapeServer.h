// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Private/EscapeNetworkBase.h"
#include "EscapeServer.generated.h"


class UEscapeMessageContrl;

UCLASS(Transient, config = Engine)
class ESCAPENETWORK_API UEscapeServer : public UEscapeNetworkBase
{
	GENERATED_UCLASS_BODY()

	virtual bool Register(UEscapeEngine* InEngine) override;
	virtual void TickDispatch(float DeltaTime) override;
	virtual void Process() override;
	
protected:
	UPROPERTY(Config)
	int32 ListenPort;

	UPROPERTY(Config)
	int32 DedicatedServerPort;

	UPROPERTY(Config)
	int32 MaxBackLog;

	UPROPERTY(Config)
	int32 MaxNetworkErrorCount;

	UPROPERTY(Transient)
	UEscapeMessageContrl* EscapeMessageContrl;

	UPROPERTY(Config)
	FString EscapeMessageContrlClassName;

	UPROPERTY()
	TSubclassOf<UEscapeMessageContrl> EscapeMessageContrlClass;

protected:
	DECLARE_DELEGATE_ThreeParams(FServerMessageDelegate, FSocket*, void*, EErrorCode);

	struct FServerMessageCallback
	{
		ELogicCode Code;
		
		UObject* Object;

		FServerMessageDelegate Delegate;
	};

	typedef TSharedPtr<FServerMessageCallback> FServerMessageCallbackPtr;

	TArray<FServerMessageCallbackPtr> MessagesCallback;

	template <typename UserClass>
	using FServerMessageCallbackThreeParamsPtr = void(UserClass::*)(FSocket*, void*, EErrorCode);

public:
	template <typename UserClass>
	inline FServerMessageCallbackPtr AddMessageCallback(ELogicCode Code, UserClass* InUserObject, FServerMessageCallbackThreeParamsPtr<UserClass> InFunc)
	{
		FServerMessageCallbackPtr MessageCallback = MakeShareable(new FServerMessageCallback());
		MessageCallback->Code = Code;
		MessageCallback->Object = InUserObject;
		MessageCallback->Delegate.BindUObject(InUserObject, InFunc);
		MessagesCallback.Add(MessageCallback);

		return MessageCallback;
	}

	void ClearMessageCallback(UObject* Object)
	{
		TArray<FServerMessageCallbackPtr>::TIterator It(MessagesCallback);
		for (; It; ++It)
		{
			FServerMessageCallbackPtr CallbackPtr = *It;
			if (CallbackPtr->Object == Object)
			{
				MessagesCallback.Remove(CallbackPtr);

				CallbackPtr.Reset();
			}
		}
	}

	void ExecuteMessageCallback(FSocket* ClientSocket, ELogicCode Code, EErrorCode Error, void* Data)
	{
		for (FServerMessageCallbackPtr CallbackPtr : MessagesCallback)
		{
			if (CallbackPtr->Code == Code)
			{
				CallbackPtr->Delegate.ExecuteIfBound(ClientSocket, Data, Error);
			}
		}
	}

protected:
	struct FEscapeSocket
	{
	public:
		FEscapeSocket(FSocket* InSocket)
			: Socket(InSocket)
			, NetworkErrorCount(0)
		{

		}

		FSocket* operator->()
		{
			return Socket;
		}

		FSocket* operator*()
		{
			return Socket;
		}

		int32 NetworkErrorCount;

	protected:

		FSocket* Socket;
	};

	TArray<FEscapeSocket> ClientsSocket;
};