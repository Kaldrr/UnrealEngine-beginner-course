#include "MyPlayerController.h"

#include "EnhancedInputComponent.h"
#include "MyCharacter.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto* const MyCharacter = Cast<AMyCharacter>(GetPawn());
	checkf(MyCharacter, TEXT("AMyPlayerController: expectedd AMyCharacter"));

	auto* const EnhancedInputComponent =
	    MyCharacter->GetComponentByClass<UEnhancedInputComponent>();
	checkf(
	    EnhancedInputComponent,
	    TEXT(
	        "EnhancedInputComponent: character doesn't support EnhancedInputComponent"));

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered,
	                                   MyCharacter, &AMyCharacter::Jump);

	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered,
	                                   MyCharacter,
	                                   &AMyCharacter::HandleLookAction);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered,
	                                   MyCharacter,
	                                   &AMyCharacter::HandleMoveAction);

	EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this,
	                                   &AMyPlayerController::HandlePauseAction);

	EnhancedInputComponent->BindAction(UseObjectAction, ETriggerEvent::Triggered,
	                                   MyCharacter,
	                                   &AMyCharacter::HandleInteractAction);
}

void AMyPlayerController::HandlePauseAction()
{
	const bool IsGamePaused = IsPaused();
	SetPause(!IsGamePaused);

	// MyCharacter should probably be cached
	// Oh well too bad
	auto* const MyCharacter = Cast<AMyCharacter>(GetPawn());
	check(MyCharacter);

	if (IsGamePaused)
	{
		MyCharacter->SetGameHUD();
	}
	else
	{
		MyCharacter->SetPauseHUD();
	}
}
