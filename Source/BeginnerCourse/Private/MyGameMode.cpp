// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"

#include "MyCharacter.h"
#include "MyPlayerController.h"

AMyGameMode::AMyGameMode()
{
	// Set it in the blueprint derived from the class 
	// It will work if we set it here as well 
	// But setting it in blueprint allows for easy editing without recompiling code

	// PlayerControllerClass = AMyPlayerController::StaticClass();
	// DefaultPawnClass = AMyCharacter::StaticClass();
}
