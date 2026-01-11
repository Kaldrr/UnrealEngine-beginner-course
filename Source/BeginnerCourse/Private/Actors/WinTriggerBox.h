#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "WinTriggerBox.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AWinTriggerBox : public AActor
{
	GENERATED_BODY()

public:
	AWinTriggerBox();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> TriggerBox;

private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp,
	                    AActor* OtherActor,
	                    UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex,
	                    bool bFromSweep,
	                    const FHitResult& SweepResult);
};
