#include "MovementPatrolComponent.h"

UMovementPatrolComponent::UMovementPatrolComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMovementPatrolComponent::BeginPlay()
{
	Super::BeginPlay();

	if (MovementCurve && TargetComponent) [[likely]]
	{
		FOnTimelineFloat ProgressFunction{};
		ProgressFunction.BindUFunction(this, FName{ "HandleTimelineUpdate" });

		Timeline.AddInterpFloat(MovementCurve, std::move(ProgressFunction));
		Timeline.SetLooping(true);

		Timeline.PlayFromStart();
	}
}

void UMovementPatrolComponent::TickComponent(
    const float DeltaTime,
    const ELevelTick TickType,
    FActorComponentTickFunction* const ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Timeline.TickTimeline(DeltaTime);
}

void UMovementPatrolComponent::HandleTimelineUpdate(const float Value) const
{
	const FVector NewLocation = FMath::Lerp(PointA, PointB, Value);
	TargetComponent->SetRelativeLocation(NewLocation);
}
