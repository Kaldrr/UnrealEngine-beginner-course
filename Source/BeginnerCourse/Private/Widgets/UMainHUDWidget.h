#pragma once

#include "Blueprint/UserWidget.h"

#include "UMainHUDWidget.generated.h"

// Abstract class is more like an interface, really
// Define what functions we want it to have, like OnHealthChanged
// but the implementation will be defined in blueprint
// We want it to be a blueprint class so we can use the designer for easier UI creation
// But we also want an API that we will be able to call from C++ code
// as our logic lives in C++

UCLASS(Abstract)
class UMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float NewHealth);

	UFUNCTION(BlueprintImplementableEvent)
	void OnCollectedOrbsChanged(uint8 NewOrbsCount);

	void ShowDamageTint();
	void ShowOrbCollectedTint();
	
	void ShowScreenMessage(const FString& Message);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void ShowDamageTintHandler(ESlateVisibility NewState);
	
	UFUNCTION(BlueprintImplementableEvent)
	void ShowOrbCollectedTintHandler(ESlateVisibility NewState);

	UFUNCTION(BlueprintImplementableEvent)
	void ShowScreenMessageHandler(const FString& Message);
	
private:
	FTimerHandle DamageTintTimer; 
	FTimerHandle OrbCollectedTintTimer;
	FTimerHandle ScreenMessageTimer;
};
