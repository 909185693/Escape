// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Object.h"
#include "Sockets.h"
#include "IPAddress.h"
#include "NetworkTypes.h"
#include "OnlineSubsystem.h"
#include "SocketSubsystem.h"
#include "EscapeNetworkModule.h"
#include "EscapeOnlineAsyncTaskManager.h"
#include "EscapeNetworkBase.generated.h"


UCLASS(Transient, config = Engine)
class UEscapeNetworkBase : public UObject
{
	GENERATED_UCLASS_BODY()

	virtual bool Register();
	virtual bool InitBase(FString& Error);
	virtual bool SendTo(FSocket* Socket, ELogicCode Code, EErrorCode Error, int32 DataSize = 0, void* Data = nullptr);
	virtual bool RecvFrom(FSocket* Socket, void*& OutData, ELogicCode& OutCode, EErrorCode& OutError);
	virtual void AddToParallelTasks(FOnlineAsyncTask* NewTask);

	virtual bool Tick(float DeltaTime) { return true; }
	virtual void Process() {}

	/// UObject 
	virtual void BeginDestroy() override;

protected:	
	/** Online socket */
	FSocket* Socket;

	/** Online socket subsystem */
	ISocketSubsystem* SocketSubsystem;

	/** Online async task runnable */
	class FEscapeOnlineAsyncTaskManager* OnlineAsyncTaskThreadRunnable;

	/** Online async task thread */
	class FRunnableThread* OnlineAsyncTaskThread;

	/** Delegate for callbacks to Tick */
	FTickerDelegate TickDelegate;

	/** Handle to various registered delegates */
	FDelegateHandle TickDelegateHandle;
};