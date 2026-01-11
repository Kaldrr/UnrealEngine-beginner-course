#include "UMainHUDWidget.h"

void UMainHUDWidget::ShowDamageTint()
{
	/*
	 * When using a lambda as a callback through the TimerManager
	 * A L W A Y S use a TWeakObjectPtr{this} and check its validity before use
	 * When using normal callback with this as a context/owner
	 * the TimerManager tracks the life of it, and won't call it when destroyed
	 * lambdas are not UObjects, and getting the `this` pointer from it is impossible
	 * so impossible to track lifetime, TimerManager just trusts us that we know
	 * what we are doing
	 * Spoiler alert: we do NOT know what we are doing
	 * when dying and reloading the level, there was a crash here
	 * because we triggered the DamageTint, AND reloaded the level which destroyed
	 * the HUD widget, invalidating the this pointer and causing a spectacular
	 * crash
	 * TWeakPointer is a workaround, lambdas and TimerManager just don't really mix
	 * together well :/
	 * At least a nice example of TWeakObjectPtr indeed working as expected
	 * without any UPROPERTY or other magic
	 */
	
	ShowDamageTintHandler(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(
	    DamageTintTimer,
	    [WeakThis = TWeakObjectPtr{ this }] {
		    if (WeakThis.IsValid())
		    {
			    WeakThis->ShowDamageTintHandler(ESlateVisibility::Hidden);
		    }
	    },
	    0.3f, false);
}

void UMainHUDWidget::ShowOrbCollectedTint()
{
	ShowOrbCollectedTintHandler(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(
	    OrbCollectedTintTimer,
	    [WeakThis = TWeakObjectPtr{ this }] {
		    if (WeakThis.IsValid())
		    {
			    WeakThis->ShowOrbCollectedTintHandler(ESlateVisibility::Hidden);
		    }
	    },
	    0.3f, false);
}

void UMainHUDWidget::ShowScreenMessage(const FString& Message)
{
	ShowScreenMessageHandler(Message);
	GetWorld()->GetTimerManager().SetTimer(
	    ScreenMessageTimer,
	    [WeakThis = TWeakObjectPtr{ this }] {
		    if (WeakThis.IsValid())
		    {
			    WeakThis->ShowScreenMessageHandler(FString{});
		    }
	    },
	    2.0f, false);
}