#pragma once

#include "Components/TimelineComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"

#include "RotatingDoor.generated.h"

UCLASS()
class ARotatingDoor
    : public AActor
    , public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARotatingDoor();

	virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleTimelineUpdate(float Value) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> DoorMesh;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> GlassMesh;

	FTimeline Timeline;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> RotationCurve;

private:
	FRotator StartingRotation;
	bool HasExecuted{ false };
};
