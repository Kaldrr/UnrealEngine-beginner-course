#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"

#include "BackgroundNoiseTrigger.generated.h"

UCLASS()
class ABackgroundNoiseTrigger : public ATriggerBox
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTriggerEnter(AActor* OverlappedActor, AActor* OtherActor);
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> BackgroundSound;
};
