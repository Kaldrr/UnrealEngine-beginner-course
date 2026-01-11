#include "BackgroundNoiseTrigger.h"

#include "GameLogic/MyCharacter.h"
#include "Kismet/GameplayStatics.h"

void ABackgroundNoiseTrigger::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ABackgroundNoiseTrigger::OnTriggerEnter);
}

void ABackgroundNoiseTrigger::OnTriggerEnter(
    [[maybe_unused]] AActor* const OverlappedActor,
    AActor* const OtherActor)
{
	if (OtherActor->IsA<AMyCharacter>())
	{
		UGameplayStatics::PlaySound2D(this, BackgroundSound);
	}
}