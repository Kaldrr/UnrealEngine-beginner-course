#include "WinTriggerBox.h"

#include "Components/BoxComponent.h"
#include "GameLogic/MyCharacter.h"

AWinTriggerBox::AWinTriggerBox()
{
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox    = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	RootComponent = TriggerBox;
}

void AWinTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	
	check(TriggerBox);
	TriggerBox->OnComponentBeginOverlap.AddDynamic(
	    this, &AWinTriggerBox::OnBeginOverlap);
}

void AWinTriggerBox::OnBeginOverlap(
    [[maybe_unused]] UPrimitiveComponent* const OverlappedComp,
    AActor* const OtherActor,
    [[maybe_unused]] UPrimitiveComponent* const OtherComp,
    [[maybe_unused]] const int32 OtherBodyIndex,
    [[maybe_unused]] const bool bFromSweep,
    [[maybe_unused]] const FHitResult& SweepResult)
{
	if (AMyCharacter* const MyCharacter = Cast<AMyCharacter>(OtherActor))
	{
		MyCharacter->TriggerGameWon();
	}
}