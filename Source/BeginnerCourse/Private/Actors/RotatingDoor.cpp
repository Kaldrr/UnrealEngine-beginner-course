#include "RotatingDoor.h"

#include "GameLogic/MyCharacter.h"

ARotatingDoor::ARotatingDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	Root          = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(RootComponent);

	const static ConstructorHelpers::FObjectFinder<UStaticMesh> DoorMeshAsset{ TEXT(
		"/Game/StarterContent/Props/SM_Door.SM_Door") };
	if (DoorMeshAsset.Succeeded())
	{
		DoorMesh->SetStaticMesh(DoorMeshAsset.Object);
	}

	GlassMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GlassMesh"));
	GlassMesh->SetupAttachment(DoorMesh);

	const static ConstructorHelpers::FObjectFinder<UStaticMesh> GlassMeshAsset{
		TEXT("/Game/StarterContent/Props/SM_GlassWindow.SM_GlassWindow")
	};
	if (GlassMeshAsset.Succeeded())
	{
		GlassMesh->SetStaticMesh(GlassMeshAsset.Object);
	}
}

void ARotatingDoor::HandleTimelineUpdate(const float Value) const
{
	const FRotator NewRotation = StartingRotation - FRotator{ 0, Value, 0 };
	DoorMesh->SetRelativeRotation(NewRotation);
}

void ARotatingDoor::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	Timeline.TickTimeline(DeltaTime);
}

void ARotatingDoor::Interact_Implementation(AActor* const Actor)
{
	if (HasExecuted)
	{
		return;
	}

	if (const AMyCharacter* const MyCharacter = Cast<AMyCharacter>(Actor);
	    !MyCharacter || MyCharacter->GetOrbsCollected() < 3)
	{
		return;
	}

	if (RotationCurve && DoorMesh) [[likely]]
	{
		StartingRotation = DoorMesh->GetRelativeRotation();

		FOnTimelineFloat ProgressFunction{};
		ProgressFunction.BindUFunction(this, FName{ "HandleTimelineUpdate" });

		Timeline.AddInterpFloat(RotationCurve, std::move(ProgressFunction));
		Timeline.SetLooping(false);

		Timeline.PlayFromStart();
		HasExecuted = true;
	}
}
