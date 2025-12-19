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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* Root;
	
	UPROPERTY(EditAnywhere, Category="Mesh")
	UStaticMeshComponent* PlatformMesh;
	
	UPROPERTY(EditAnywhere, Category="Timeline")
	UCurveFloat* PositionCurve;
	
	FTimeline PositionTimeline;
	
	UPROPERTY(EditAnywhere, Category="Timeline", meta=(MakeEditWidget))
	FVector TargetLocation;
};
