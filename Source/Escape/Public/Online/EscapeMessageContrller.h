// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "Object.h"
#include "EscapeNetwork.h"
#include "EscapeMessageContrller.generated.h"


UCLASS()
class ESCAPE_API UEscapeMessageContrller : public UObject
{
	GENERATED_UCLASS_BODY()
	
	virtual void ConnectionCallback(void* Data, EErrorCode Error);
	virtual void UserLoginCallback(void* Data, EErrorCode Error);
};
