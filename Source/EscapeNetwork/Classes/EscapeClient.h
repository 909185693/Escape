// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Private/EscapeNetworkBase.h"
#include "EscapeClient.generated.h"


UCLASS(Transient, config = Engine)
class ESCAPENETWORK_API UEscapeClient : public UEscapeNetworkBase
{
	GENERATED_UCLASS_BODY()

	virtual bool Register(UEscapeEngine* InEngine) override;
	virtual void Process() override;

	/// EscapeClient
public:
	virtual void TickDispatch(float DeltaTime) override;
	virtual void Send(ELogicCode Code, int32 DataSize = 0, void* Data = nullptr);
	virtual void Reconnect();

public:
	virtual void BeginDestroy() override;

protected:
	DECLARE_DELEGATE_TwoParams(FClientMessageDelegate, void*, EErrorCode);

	struct FClientMessageCallback
	{
		ELogicCode Code;

		UObject* Object;

		FClientMessageDelegate Delegate;
	};

	typedef TSharedPtr<FClientMessageCallback> FClientMessageCallbackPtr;

	TArray<FClientMessageCallbackPtr> MessagesCallback;

	template <typename UserClass>
	using FClientMessageCallbackTwoParamsPtr = void(UserClass::*)(void*, EErrorCode);

public:
	template <typename UserClass>
	inline FClientMessageCallbackPtr AddMessageCallback(ELogicCode Code, UserClass* InUserObject, FClientMessageCallbackTwoParamsPtr<UserClass> InFunc)
	{
		FClientMessageCallbackPtr MessageCallback = MakeShareable(new FClientMessageCallback());
		MessageCallback->Code = Code;
		MessageCallback->Delegate.BindUObject(InUserObject, InFunc);
		MessagesCallback.Add(MessageCallback);

		return MessageCallback;
	}

	void ClearMessageCallback(UObject* Object)
	{
		TArray<FClientMessageCallbackPtr>::TIterator It(MessagesCallback);
		for ( ; It; ++It)
		{
			FClientMessageCallbackPtr CallbackPtr = *It;
			if (CallbackPtr->Object == Object)
			{
				MessagesCallback.Remove(CallbackPtr);
			}
		}
	}

protected:
	struct FMessageData
	{
	public:
		FMessageData(void* InData, ELogicCode InCode, EErrorCode InError)
			: Data(InData)
			, Code(InCode)
			, Error(InError)
		{

		}

		FMessageData()
			: Data(nullptr)
			, Code(ELogicCode::INVALID)
			, Error(EErrorCode::NONE)
		{

		}

		void* Data;

		ELogicCode Code;

		EErrorCode Error;
	};

	TQueue<FMessageData, EQueueMode::Mpsc> MessageQueue;

	virtual void AddMessage(void* Data, ELogicCode Code, EErrorCode Error);

protected:
	UPROPERTY(Config)
	FString ServerIP;

	UPROPERTY(Config)
	int32 ServerPort;

	int32 ConnectCount;

	UPROPERTY(Config)
	int32 MaxConnectCount;

	bool bShouldConnected;

	bool bIsConnected;
};