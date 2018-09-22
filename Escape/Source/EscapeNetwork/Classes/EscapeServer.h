// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Private/EscapeNetworkBase.h"
#include "EscapeServer.generated.h"


class UEscapeMessageContrl;

struct FConnection
{
public:
	FConnection(FSocket* InSocket)
		: Socket(InSocket)
		, NetworkErrorCount(0)
	{

	}

	FConnection()
		: Socket(nullptr)
		, NetworkErrorCount(0)
	{

	}

	bool operator==(const FConnection& Other) const
	{
		return Socket == Other.Socket;
	}
	
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
	DECLARE_DELEGATE_ThreeParams(FServerMessageDelegate, TSharedPtr<FConnection>, void*, EErrorCode);

	struct FServerMessageCallback
	{
		ELogicCode Code;
		
		UObject* Object;

		FServerMessageDelegate Delegate;
	};
	typedef TSharedPtr<FServerMessageCallback> FServerMessageCallbackPtr;

	TArray<FServerMessageCallbackPtr> MessagesCallback;

	template <typename UserClass>
	using FServerMessageCallbackThreeParamsPtr = void(UserClass::*)(TSharedPtr<FConnection>, void*, EErrorCode);

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
			}
		}
	}

	void ExecuteMessageCallback(TSharedPtr<FConnection> Connection, ELogicCode Code, EErrorCode Error, void* Data)
	{
		for (FServerMessageCallbackPtr CallbackPtr : MessagesCallback)
		{
			if (CallbackPtr->Code == Code)
			{
				CallbackPtr->Delegate.ExecuteIfBound(Connection, Data, Error);
			}
		}
	}

	void CloseConnection(TSharedPtr<FConnection> Connection);

	template<typename Struct> bool RegisterConnection(TSharedPtr<FConnection> Source, const TSharedPtr<Struct> Target)
	{
		Target->InitConnection(*Source);

		TSharedPtr<FConnection> TargetToAdd = ConstCastSharedPtr<Struct>(Target);
		if (TargetToAdd.IsValid())
		{
			Connections.Remove(Source);
			Connections.Add(TargetToAdd);
			
			return true;
		}

		return false;
	}
};