// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "EscapeWidget.h"
#include "EscapeUserLoginWidget.generated.h"


UENUM(BlueprintType)
enum EUserLogin
{
	Login_Success,
	Login_Failed
};

UCLASS()
class ESCAPE_API UEscapeUserLoginWidget : public UEscapeWidget
{
	GENERATED_UCLASS_BODY()
	
protected:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "NotifyNotifyUserLogin"))
	void ReceiveNotifyUserLogin(EUserLogin NetworkConnection);

	UFUNCTION(BlueprintCallable, Category = "UserLogin")
	void UserLogin(const FText& Username, const FText& Password);

protected:
	virtual void BindEscapeClientEvent() override;
	virtual void NotifyNotifyUserLogin(void* Data, EErrorCode Error);
	
	
};
