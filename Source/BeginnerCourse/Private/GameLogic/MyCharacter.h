#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"

#include "MyCharacter.generated.h"

UCLASS()
class AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyCharacter();

	virtual float TakeDamage(float Damage,
	                         const FDamageEvent& DamageEvent,
	                         AController* EventInstigator,
	                         AActor* DamageCauser) override;

	// InputAction handlers
	void HandleLookAction(const FInputActionValue& LookAxis);
	void HandleMoveAction(const FInputActionValue& MoveAxis);
	void HandleInteractAction();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float InvincibilityDuration = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float Health = 1.0f;

private:
	void EndInvincibility();

	FTimerHandle InvincibilityTimerHandle;
	bool IsInvincible{ false };
};
