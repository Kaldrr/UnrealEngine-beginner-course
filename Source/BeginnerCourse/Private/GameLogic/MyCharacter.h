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
	
	void SetPauseHUD();
	void SetGameHUD();
	
	void TriggerGameOver();
	void TriggerGameWon();
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

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

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseHUDClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameOverHUDClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameWonHUDClass;
	
private:
	void EndInvincibility() noexcept;

	void UpdateInteractionPrompt();
	void HandleLineTraceCheck(const FTraceHandle& TraceHandle,
	                          FTraceDatum& TraceDatum);
	void SwitchToGameScreen(UUserWidget& NewWidget);

	FTimerHandle InvincibilityTimerHandle;
	bool IsInvincible{ false };
	std::uint8_t OrbsCollected{ 0 };

	UPROPERTY()
	TObjectPtr<UMainHUDWidget> MainHUD;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> PauseHUD;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> GameOverHUD;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> GameWonHUD;


	FTimerHandle InteractionTimerHandle;
	TWeakObjectPtr<AActor> FocusedActor{};
};
