#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "Widgets/UMainHUDWidget.h"

#include "MyCharacter.generated.h"

class UMainHUDWidget;

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

	void CollectOrb() noexcept;

	[[nodiscard]] std::uint8_t GetOrbsCollected() const noexcept
	{
		return OrbsCollected;
	}

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

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UMainHUDWidget> HUDClass;

private:
	void EndInvincibility() noexcept;

	FTimerHandle InvincibilityTimerHandle;
	bool IsInvincible{ false };
	std::uint8_t OrbsCollected{ 0 };

	UPROPERTY()
	TObjectPtr<UMainHUDWidget> MainHUD;
};
