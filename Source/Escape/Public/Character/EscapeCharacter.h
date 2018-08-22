// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EscapeCharacter.generated.h"

UCLASS()
class ESCAPE_API AEscapeCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEscapeCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	* Make the character jump on the next update.
	* If you want your character to jump according to the time that the jump key is held,
	* then you can set JumpKeyHoldTime to some non-zero value. Make sure in this case to
	* call StopJumping() when you want the jump's z-velocity to stop being applied (such
	* as on a button up event), otherwise the character will carry on receiving the
	* velocity until JumpKeyHoldTime is reached.
	*/
	virtual void Jump() override;
	
};
