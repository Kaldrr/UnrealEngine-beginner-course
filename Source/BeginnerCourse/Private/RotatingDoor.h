// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "RotatingDoor.generated.h"

UCLASS()
class ARotatingDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotatingDoor();
	
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleTimelineUpdate(float Value) const;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(EditAnywhere, Category="Mesh")
	TObjectPtr<UStaticMeshComponent> DoorMesh;
	
	UPROPERTY(EditAnywhere, Category="Mesh")
	TObjectPtr<UStaticMeshComponent> GlassMesh;
	
	FTimeline Timeline;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> RotationCurve;
	
private:
	FRotator StartingRotation;
};
