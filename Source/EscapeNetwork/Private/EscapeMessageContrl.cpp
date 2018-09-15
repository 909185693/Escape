// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "EscapeMessageContrl.h"


UEscapeMessageContrl::UEscapeMessageContrl(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
	, EscapeServer(nullptr)
{

}

void UEscapeMessageContrl::Register(UEscapeServer* InEscapeServer)
{
	EscapeServer = InEscapeServer;
	if (EscapeServer != nullptr)
	{

	}
}

void UEscapeMessageContrl::BeginDestroy()
{

}