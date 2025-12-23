#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "LevelPlatform.generated.h"

UCLASS()
class ALevelPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	ALevelPlatform();

	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleTimelineUpdate(float Value);
	
	// TObjectPtr -> UObject-aware "smart" pointer, works with GC
	// Not ownership, as that's represented by UPROPERTY
	// and UE can better track, relocate, validate etc. it
	// best to use for class members
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(EditAnywhere, Category="Mesh")
	TObjectPtr<UStaticMeshComponent> PlatformMesh;
	
	UPROPERTY(EditAnywhere, Category="Timeline")
	TObjectPtr<UCurveFloat> PositionCurve;
	
	// Value, not a pointer, but we want to edit it in editor
	// UPROPERTY(EditAnywhere) allows that
	// Category places in 'Timeline' group
	// meta=(MakeEditWidget) gives a 3d 'diamond' that shows the vector
	UPROPERTY(EditAnywhere, Category="Timeline", meta=(MakeEditWidget))
	FVector TargetLocation;

	// Value, not a pointer, no need for UPROPERTY
	FTimeline PositionTimeline;
};
