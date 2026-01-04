// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GameOverTriggerVolume.generated.h"

UCLASS()
class AGameOverTriggerVolume : public AActor
{
	GENERATED_BODY()

public:
	AGameOverTriggerVolume();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp,
	                    AActor* OtherActor,
	                    UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex,
	                    bool bFromSweep,
	                    const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> TriggerBox;
};
