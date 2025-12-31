#include "LevelPlatform.h"

ALevelPlatform::ALevelPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create a default root component for easier positioning on the scene
	// and movement of the platform mesh, it will be relative to root
	Root          = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	PlatformMesh =
	    CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	PlatformMesh->SetupAttachment(Root);

	MovementComponent =
	    CreateDefaultSubobject<UMovementPatrolComponent>(TEXT("MovementComponent"));
	MovementComponent->TargetComponent = PlatformMesh;
}

void ALevelPlatform::BeginPlay()
{
	Super::BeginPlay();
}

void ALevelPlatform::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}
