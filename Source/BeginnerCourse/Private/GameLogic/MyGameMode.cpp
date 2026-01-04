#include "MyGameMode.h"

#include "Kismet/KismetSystemLibrary.h"

// #include "MyCharacter.h"
// #include "MyPlayerController.h"

AMyGameMode::AMyGameMode()
{
	// Set it in the blueprint derived from the class
	// It will work if we set it here as well
	// But setting it in blueprint allows for easy editing without recompiling code

	// PlayerControllerClass = AMyPlayerController::StaticClass();
	// DefaultPawnClass = AMyCharacter::StaticClass();
}

void AMyGameMode::TriggerGameOver() const
{
	UE_LOG(LogTemp, Warning, TEXT("Game Over"));
	OnGameOver.Broadcast();
}

void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();

	OnGameOver.AddDynamic(this, &AMyGameMode::GameOverHandler);
}

void AMyGameMode::GameOverHandler()
{
	if (const UWorld* const World = GetWorld())
	{
		UKismetSystemLibrary::QuitGame(World, World->GetFirstPlayerController(),
		                               EQuitPreference::Quit, false);
	}
}
