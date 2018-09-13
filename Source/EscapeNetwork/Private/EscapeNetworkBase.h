// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Object.h"
#include "Sockets.h"
#include "IPAddress.h"
#include "OnlineSubsystem.h"
#include "SocketSubsystem.h"
#include "EscapeNetworkTypes.h"
#include "EscapeNetworkModule.h"
#include "EscapeNetworkBase.generated.h"


DECLARE_DELEGATE_TwoParams(FMessageDelegate, void*, EErrorCode);

struct FMessageCallback
{
	ELogicCode Code;

	EErrorCode Error;

	FMessageDelegate MessageDelegate;
};

class FEscapeNetworkNotify;
class FEscapeOnlineAsyncTaskManager;

UCLASS(Transient, config = Engine)
class UEscapeNetworkBase : public UObject
{
	GENERATED_UCLASS_BODY()

	virtual bool Run();
	virtual bool InitBase(FString& Error);
	virtual bool SendTo(FSocket* Socket, ELogicCode Code, int32 DataSize, void* Data);
	virtual bool RecvFrom(FSocket* Socket, void* OutData, ELogicCode& OutCode, EErrorCode& OutError);
	virtual void TickDispatch(float DeltaTime) PURE_VIRTUAL(UEscapeNetworkBase::TickDispatch, );
	virtual void Process() PURE_VIRTUAL(UEscapeNetworkBase::Process, );
	virtual void SetNetworkNotify(FEscapeNetworkNotify* NetworkNotify);

	template <typename UserClass>
	inline void AddMessageCallback(UserClass* InUserObject, void(UserClass::*)(void*, EErrorCode))
	{
		FMessageCallback MessageCallback;
		//MessageCallback.MessageDelegate.BindUObject(InUserObject, InFunc);

		MessageCallbacks.Add(MessageCallback);
	}

	/// UObject 
	virtual void BeginDestroy() override;

protected:
	FSocket* Socket;

	ISocketSubsystem* SocketSubsystem;

	/** Online async task runnable */
	class FEscapeOnlineAsyncTaskManager* OnlineAsyncTaskThreadRunnable;

	/** Online async task thread */
	class FRunnableThread* OnlineAsyncTaskThread;

	TArray<FMessageCallback> MessageCallbacks;
};