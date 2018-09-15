// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "EscapeWidget.h"
#include "EscapeLobbyWidget.generated.h"



UCLASS()
class ESCAPE_API UEscapeLobbyWidget : public UEscapeWidget
{
	GENERATED_UCLASS_BODY()
	
protected:
	UFUNCTION(BlueprintCallable, Category = "UserLogin")
	void UserLogin(const FText& Username, const FText& Password);

	UFUNCTION(BlueprintCallable, Category = "MatchGame")
	void MatchGame();

	UFUNCTION(BlueprintCallable, Category = "MatchGame")
	void Invitation(const int32 UserID);

protected:
	virtual void RegisterMessageCallback() override;
	virtual void UnregisterMessageCallback() override;

	/// �û���¼
	FMessageCallbackPtr NotifyUserLoginPtr;
	void NotifyUserLogin(void* Data, EErrorCode Error);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "NotifyUserLogin"))
	void ReceiveNotifyUserLogin(bool bSuccess);

	/// ƥ����Ϸ
	FMessageCallbackPtr NotifyMatchGamePtr;
	void NotifyMatchGame(void* Data, EErrorCode Error);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "NotifyMatchGame"))
	void ReceiveNotifyMatchGame(bool bSuccess);

	/// ����
	FMessageCallbackPtr NotifyInvitationPtr;
	void NotifyInvitation(void* Data, EErrorCode Error);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "NotifyMatchGame"))
	void ReceiveNotifyInvitation(bool bSuccess);
};
