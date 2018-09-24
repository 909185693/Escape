// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Object.h"
#include "Sockets.h"
#include "IPAddress.h"
#include "OnlineSubsystem.h"
#include "SocketSubsystem.h"
#include "NetworkTypes.h"
#include "EscapeEngine.h"
#include "EscapeNetworkModule.h"
#include "EscapeOnlineAsyncTaskManager.h"
#include "EscapeNetworkBase.generated.h"


UCLASS(Transient, config = Engine)
class UEscapeNetworkBase : public UObject
{
	GENERATED_UCLASS_BODY()

	virtual bool Register(UEscapeEngine* InEngine);
	virtual bool InitBase(FString& Error);
	virtual bool SendTo(FSocket* Socket, ELogicCode Code, EErrorCode Error, int32 DataSize = 0, void* Data = nullptr);
	virtual bool RecvFrom(FSocket* Socket, void*& OutData, ELogicCode& OutCode, EErrorCode& OutError);
	virtual void TickDispatch(float DeltaTime) PURE_VIRTUAL(UEscapeNetworkBase::TickDispatch, );
	virtual void Process() PURE_VIRTUAL(UEscapeNetworkBase::Process, );
	virtual void AddToParallelTasks(FOnlineAsyncTask* NewTask);
	virtual class UEscapeEngine* GetEngine() const;

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
};