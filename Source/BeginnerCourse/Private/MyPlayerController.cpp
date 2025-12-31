// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyCharacter.h"
#include "EnhancedInputComponent.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto* const MyCharacter = Cast<AMyCharacter>(GetPawn());
	if (!MyCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("AMyPlayerController: Unexpected pawn class"));
	}

	if (auto* const InputComponent = MyCharacter->GetComponentByClass<UEnhancedInputComponent>();
		InputComponent)
	{
		InputComponent->BindAction(
			JumpAction,
			ETriggerEvent::Triggered,
			MyCharacter,
			&AMyCharacter::Jump);

		InputComponent->BindAction(
			LookAction,
			ETriggerEvent::Triggered,
			MyCharacter,
			&AMyCharacter::HandleLookAction);

		InputComponent->BindAction(
			MoveAction,
			ETriggerEvent::Triggered,
			MyCharacter,
			&AMyCharacter::HandleMoveAction);

		InputComponent->BindAction(
			PauseAction,
			ETriggerEvent::Triggered,
			this,
			&AMyPlayerController::HandlePauseAction);

		InputComponent->BindAction(
			UseObjectAction,
			ETriggerEvent::Triggered,
			MyCharacter,
			&AMyCharacter::HandleInteractAction);
	}
}

void AMyPlayerController::HandlePauseAction()
{
	const bool IsGamePaused = IsPaused();
	SetPause(!IsGamePaused);
}
