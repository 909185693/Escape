// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Private/EscapeNetworkBase.h"
#include "EscapeServer.generated.h"


class UEscapeMessageContrl;

struct FConnection
{
public:
	FConnection(FSocket* InSocket)
		: UserID(0)
		, Socket(InSocket)
		, NetworkErrorCount(0)
	{

	}

	FSocket* operator->()
	{
		return Socket;
	}

	FSocket* operator*() const
	{
		return Socket;
	}

	bool operator==(const FConnection& Other) const
	{
		return Socket == *Other;
	}

	int32 UserID;

	FSocket* Socket;

	int32 NetworkErrorCount;
};

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

	TArray<TSharedPtr<FConnection>> Connections;

protected:
	DECLARE_DELEGATE_ThreeParams(FServerMessageDelegate, FConnection&, void*, EErrorCode);

	struct FServerMessageCallback
	{
		ELogicCode Code;
		
		UObject* Object;

		FServerMessageDelegate Delegate;
	};
	typedef TSharedPtr<FServerMessageCallback> FServerMessageCallbackPtr;

	TArray<FServerMessageCallbackPtr> MessagesCallback;

	template <typename UserClass>
	using FServerMessageCallbackThreeParamsPtr = void(UserClass::*)(FConnection&, void*, EErrorCode);

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

	void ExecuteMessageCallback(FConnection& Connection, ELogicCode Code, EErrorCode Error, void* Data)
	{
		for (FServerMessageCallbackPtr CallbackPtr : MessagesCallback)
		{
			if (CallbackPtr->Code == Code)
			{
				CallbackPtr->Delegate.ExecuteIfBound(Connection, Data, Error);
			}
		}
	}

	void CloseConnection(FConnection& Connection);
};