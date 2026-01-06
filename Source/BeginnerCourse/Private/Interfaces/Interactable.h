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

// TODO: This REALLY should work with another interface for the source of interaction
// right now we just cast to AMyCharacter, as it is the ONLY possible source
// of interaction in the game, but in real games other actors could need such
// functionality as well
// IInteractSource/IInteractor which would have logic if given interactable 
// can be used
// Or, something like FInteractContext, with common data that could be used by 
// interaction logic
// For this project a simple cast is ok, but there is nothing more permanent than
// a temporary solution/hack :^)

// The REAL C++ interface with functionality
class IInteractable
{
	GENERATED_BODY()

public:
	// Interact is a stub, managed by Unreal and implementable in Blueprints
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Trigger)
	void Interact(AActor* Actor);

	// C++ side function, that we implement to provide the logic on C++ side
	// we also Call IInteractable::Execute_Interact, not functions directly!
	virtual void Interact_Implementation(AActor* Actor) = 0;
};
