// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Object.h"
#include "Sockets.h"
#include "IPAddress.h"
#include "OnlineSubsystem.h"
#include "SocketSubsystem.h"
#include "EscapeEngine.h"
#include "EscapeNetworkTypes.h"
#include "EscapeNetworkModule.h"
#include "EscapeNetworkBase.generated.h"


DECLARE_DELEGATE_TwoParams(FMessageDelegate, void*, EErrorCode);

struct FMessageCallback
{
	ELogicCode Code;

	FMessageDelegate MessageDelegate;
};

class FEscapeNetworkNotify;
class FEscapeOnlineAsyncTaskManager;

UCLASS(Transient, config = Engine)
class UEscapeNetworkBase : public UObject
{
	GENERATED_UCLASS_BODY()

	virtual bool Register(UEscapeEngine* InEngine);
	virtual bool InitBase(FString& Error);
	virtual bool SendTo(FSocket* Socket, ELogicCode Code, int32 DataSize, void* Data);
	virtual bool RecvFrom(FSocket* Socket, void*& OutData, ELogicCode& OutCode, EErrorCode& OutError);
	virtual void TickDispatch(float DeltaTime) PURE_VIRTUAL(UEscapeNetworkBase::TickDispatch, );
	virtual void Process() PURE_VIRTUAL(UEscapeNetworkBase::Process, );
	virtual class UEscapeEngine* GetEngine() const;

	template <typename UserClass>
	using FMessageCallbackTwoParamsPtr = void(UserClass::*)(void*, EErrorCode);

	template <typename UserClass>
	inline void AddMessageCallback(ELogicCode Code, UserClass* InUserObject, FMessageCallbackTwoParamsPtr<UserClass> InFunc)
	{
		FMessageCallback MessageCallback;
		MessageCallback.Code = Code;
		MessageCallback.MessageDelegate.BindUObject(InUserObject, InFunc);
		MessagesCallback.Add(MessageCallback);
	}

	/// UObject 
	virtual void BeginDestroy() override;

protected:
	UPROPERTY(Transient)
	UEscapeEngine* Engine;
	
	FSocket* Socket;

	ISocketSubsystem* SocketSubsystem;

	/** Online async task runnable */
	class FEscapeOnlineAsyncTaskManager* OnlineAsyncTaskThreadRunnable;

	/** Online async task thread */
	class FRunnableThread* OnlineAsyncTaskThread;

	/** Handles to various registered delegates */
	FDelegateHandle TickDispatchDelegateHandle;

	TArray<FMessageCallback> MessagesCallback;
};