#include "LevelPlatform.h"

#include "Components/TimelineComponent.h"

ALevelPlatform::ALevelPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Create a default root component for easier positioning on the scene
	// and movement of the platform mesh, it will be relative to root
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	PlatformMesh->SetupAttachment(Root);
}

void ALevelPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	if (PositionCurve)
	{
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindDynamic(this, &ALevelPlatform::HandleTimelineUpdate);
		
		PositionTimeline.AddInterpFloat(PositionCurve, std::move(ProgressFunction));
		PositionTimeline.SetLooping(true);
		
		PositionTimeline.PlayFromStart();
	}
}

void ALevelPlatform::HandleTimelineUpdate(const float Value)
{
	if (!IsValid(PlatformMesh)) [[unlikely]]
	{
		return;
	}
	const FVector NewLocation = FMath::Lerp(
		FVector{0.0, 0.0, 0.0}, // Position relative to the parent
		TargetLocation,
		Value
	);
	PlatformMesh->SetRelativeLocation(NewLocation);
}

void ALevelPlatform::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	PositionTimeline.TickTimeline(DeltaTime);
}

