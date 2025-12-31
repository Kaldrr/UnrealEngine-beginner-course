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
};
