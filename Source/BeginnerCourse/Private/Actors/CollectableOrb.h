#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"

#include "CollectableOrb.generated.h"

UCLASS()
class ACollectableOrb : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:
	ACollectableOrb();
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
						AActor* OtherActor,
						UPrimitiveComponent* OtherComp,
						int32 OtherBodyIndex,
						bool bFromSweep,
						const FHitResult& SweepResult);
};
