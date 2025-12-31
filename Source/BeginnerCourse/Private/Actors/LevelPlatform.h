#pragma once

#include "Components/MovementPatrolComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "LevelPlatform.generated.h"

UCLASS()
class ALevelPlatform : public AActor
{
	GENERATED_BODY()

public:
	ALevelPlatform();

protected:
	// TObjectPtr -> UObject-aware pointer, works with GC
	// Not ownership, as that's represented by UPROPERTY
	// and UE can better track, relocate, validate etc. it
	// best to use for class members

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> PlatformMesh;

	UPROPERTY(EditAnywhere, Category = "Movement")
	TObjectPtr<UMovementPatrolComponent> MovementComponent;
};
