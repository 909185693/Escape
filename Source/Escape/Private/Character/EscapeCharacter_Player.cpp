// Copyright 2018 by January. All Rights Reserved.

#include "EscapeCharacter_Player.h"
#include "GameFramework/PlayerInput.h"


AEscapeCharacter_Player::AEscapeCharacter_Player(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bAddDefaultMovementBindings(true)
	, BaseTurnRate(45.f)
	, BaseLookUpRate(45.f)

{
	FirstPressedDelay = 0.2f;
}

void AEscapeCharacter_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocallyControlled() && bAttackPressed)
	{
		if (TimeSeconds - LastPressedTime >= FirstPressedDelay)
		{
			Attack();
		}
	}
}

namespace EscapeInputBindings
{
	void InitializeDefaultPawnInputBindings()
	{
		static bool bBindingsAdded = false;
		if (!bBindingsAdded)
		{
			bBindingsAdded = true;

			UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::W, 1.f));
			UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::S, -1.f));
			UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::Up, 1.f));
			UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::Down, -1.f));
			UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::Gamepad_LeftY, 1.f));

			UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveRight", EKeys::A, -1.f));
			UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveRight", EKeys::D, 1.f));
			UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveRight", EKeys::Gamepad_LeftX, 1.f));

			UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("TurnRate", EKeys::Gamepad_RightX, 1.f));
			UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("TurnRate", EKeys::Left, -1.f));
			UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("TurnRate", EKeys::Right, 1.f));
			UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Turn", EKeys::MouseX, 1.f));

			UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("LookUpRate", EKeys::Gamepad_RightY, 1.f));
			UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("LookUp", EKeys::MouseY, -1.f));

			UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Jump", EKeys::SpaceBar));
			UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Attack", EKeys::LeftMouseButton));
			UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Attack", EKeys::TouchKeys[0]));
			UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Attack", EKeys::TouchKeys[1]));
		}
	}
}

void AEscapeCharacter_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	if (bAddDefaultMovementBindings)
	{
		EscapeInputBindings::InitializeDefaultPawnInputBindings();

		PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
		PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

		PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AEscapeCharacter_Player::StartAttack);
		PlayerInputComponent->BindAction("Attack", IE_Released, this, &AEscapeCharacter_Player::StopAttacking);

		PlayerInputComponent->BindAxis("MoveForward", this, &AEscapeCharacter_Player::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", this, &AEscapeCharacter_Player::MoveRight);

		// We have 2 versions of the rotation bindings to handle different kinds of devices differently
		// "turn" handles devices that provide an absolute delta, such as a mouse.
		// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
		PlayerInputComponent->BindAxis("Turn", this, &AEscapeCharacter_Player::AddControllerYawInput);
		PlayerInputComponent->BindAxis("TurnRate", this, &AEscapeCharacter_Player::TurnAtRate);
		PlayerInputComponent->BindAxis("LookUp", this, &AEscapeCharacter_Player::AddControllerPitchInput);
		PlayerInputComponent->BindAxis("LookUpRate", this, &AEscapeCharacter_Player::LookUpAtRate);
	}
}

void AEscapeCharacter_Player::MoveRight(float Val)
{
	if (Val != 0.f)
	{
		if (Controller)
		{
			FRotator const ControlSpaceRot = Controller->GetControlRotation();

			// transform to world space and add it
			AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::Y), Val * CustomTimeDilation);
		}
	}
}

void AEscapeCharacter_Player::MoveForward(float Val)
{
	if (Val != 0.f)
	{
		if (Controller)
		{
			FRotator const ControlSpaceRot = Controller->GetControlRotation();

			// transform to world space and add it
			AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::X), Val * CustomTimeDilation);
		}
	}
}

void AEscapeCharacter_Player::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
}

void AEscapeCharacter_Player::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
}

void AEscapeCharacter_Player::StartAttack()
{
	bAttackPressed = true;

	LastPressedTime = TimeSeconds;

	Attack();
}

void AEscapeCharacter_Player::StopAttacking()
{
	bAttackPressed = false;
}

void AEscapeCharacter_Player::SetAttackPressed(bool bPressed)
{
	bAttackPressed = bPressed;
}
