#pragma once

#include "UObject/Interface.h"

#include "Interactable.generated.h"

// A tag class, for Unreal reflection system
// DO NOT PUT FUNCTIONS HERE
UINTERFACE(MinimalAPI, Blueprintable)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

// The REAL C++ interface with functionality
class IInteractable
{
	GENERATED_BODY()

public:
	// Interact is a stub, managed by Unreal and implementable in Blueprints
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Trigger)
	void Interact();

	// C++ side function, that we implement to provide the logic on C++ side
	// we also Call IInteractable::Execute_Interact, not functions directly!
	virtual void Interact_Implementation() = 0;
};
