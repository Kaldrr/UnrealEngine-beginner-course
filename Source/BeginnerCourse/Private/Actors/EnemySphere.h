#pragma once

#include "Components/MovementPatrolComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"

#include "EnemySphere.generated.h"

UCLASS()
class AEnemySphere : public AActor
{
	GENERATED_BODY()

public:
	AEnemySphere();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> SphereMesh;

	// Course uses particle system, but it's been deprecated in favor of Niagara
	// So we use that instead, but it doesn't have a default/begginer content fire
	// So I had to make one myself
	// This was a huge mistake D:
	UPROPERTY(EditAnywhere, Category = "Particle")
	TObjectPtr<UNiagaraComponent> FireParticleSystem;

	UPROPERTY(EditAnywhere, Category = "Movement")
	TObjectPtr<UMovementPatrolComponent> MovementComponent;
};
