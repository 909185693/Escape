// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "EscapeWidget.h"
#include "EscapeLobbyWidget.generated.h"


UENUM(BlueprintType)
enum EUserLogin
{
	Login_Success,
	Login_NetworkError,
	Login_PasswordError,
	Login_NoneError,
};

USTRUCT(BlueprintType)
struct FEscapeUser
{
	GENERATED_USTRUCT_BODY()

	FEscapeUser()
	{
		FMemory::Memzero(this, sizeof(FEscapeUser));
	}

	UPROPERTY(BlueprintReadOnly)
	int32 ID;

	UPROPERTY(BlueprintReadOnly)
	FString Nickname;
};

UCLASS()
class ESCAPE_API UEscapeLobbyWidget : public UEscapeWidget
{
	GENERATED_UCLASS_BODY()
	
protected:
	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void UserLogin(const FText& Username, const FText& Password);

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void MatchGame();

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void CancelMatch();

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void Invitation(const int32 UserID);

protected:
	virtual void RegisterMessageCallback() override;

	/// ÓÃ»§µÇÂ¼
	void NotifyUserLogin(void* Data, EErrorCode Error);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "NotifyUserLogin"))
	void ReceiveNotifyUserLogin(EUserLogin NotifyCode, FEscapeUser EscapeUser);

	/// ÑûÇë
	void NotifyInvitation(void* Data, EErrorCode Error);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "NotifyInvitation"))
	void ReceiveNotifyInvitation(bool bSuccess);
};
