// Fill out your copyright notice in the Description page of Project Settings.

#include "GameOverTriggerVolume.h"

#include "GameLogic/MyCharacter.h"
#include "GameLogic/MyGameMode.h"

AGameOverTriggerVolume::AGameOverTriggerVolume()
{
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox    = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	RootComponent = TriggerBox;
}

void AGameOverTriggerVolume::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(
	    this, &AGameOverTriggerVolume::OnBeginOverlap);
}

void AGameOverTriggerVolume::OnBeginOverlap(
    [[maybe_unused]] UPrimitiveComponent* const OverlappedComp,
    AActor* const OtherActor,
    [[maybe_unused]] UPrimitiveComponent* const OtherComp,
    [[maybe_unused]] const int32 OtherBodyIndex,
    [[maybe_unused]] const bool bFromSweep,
    [[maybe_unused]] const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AMyCharacter>())
	{
		if (const UWorld* const World = GetWorld())
		{
			if (AMyGameMode* const GameMode = World->GetAuthGameMode<AMyGameMode>())
			{
				GameMode->TriggerGameOver();
			}
		}
	}
}
