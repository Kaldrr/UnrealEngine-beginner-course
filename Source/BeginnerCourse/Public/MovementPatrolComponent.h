// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "MovementPatrolComponent.generated.h"


UCLASS(ClassGroup=(Movement), meta=(BlueprintSpawnableComponent))
class BEGINNERCOURSE_API UMovementPatrolComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMovementPatrolComponent();

	virtual void TickComponent(float DeltaTime,
	                           ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> TargetComponent;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleTimelineUpdate(float Value) const;

	FTimeline Timeline;

	// Value, not a pointer, but we want to edit it in editor
	// UPROPERTY(EditAnywhere) allows that
	// Category places in 'Timeline' group
	// meta=(MakeEditWidget) gives a 3d 'diamond' that shows the vector
	UPROPERTY(EditAnywhere, meta=(MakeEditWidget))
	FVector PointA;

	UPROPERTY(EditAnywhere, meta=(MakeEditWidget))
	FVector PointB;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> MovementCurve;
};
