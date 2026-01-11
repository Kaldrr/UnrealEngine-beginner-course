#include "MyCharacter.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameLogic/MyGameMode.h"
#include "Interfaces/Interactable.h"

namespace
{
using SecondsInterval = std::chrono::duration<float, std::chrono::seconds::period>;

constexpr float HealthEpsilon = 0.001f;
constexpr SecondsInterval InteractionCheckInterval{ 0.1f };
} // namespace

AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Add camera as a component in C++
	// But leave editing its properties to a blueprint subclass
	// Much easier iteration when we will mess around with the camera settings
	CameraComponent =
	    CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(RootComponent);
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Starting a game where some of the checks failed, like, PlayerController is null
	// doesn't really make sense, if-checks with errors logs wouldn't fix it
	// check/checkf is Unreal-style assert, crash the game as early as possible
	// there's also CheckForErrors usually hidden behind WITH_EDITOR
	// but that only works on actors placed in the level
	// Character is created/spawned by the GameMode, so it wouldn't work :(

	const auto* const PlayerController = Cast<APlayerController>(GetController());
	checkf(PlayerController != nullptr,
	       TEXT("PlayerController must be a APlayerController"));

	const auto* const LocalPlayer = PlayerController->GetLocalPlayer();
	checkf(PlayerController->GetLocalPlayer() != nullptr,
	       TEXT("Failed to get a LocalPlayer"));

	auto* const InputSubsystem =
	    LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	checkf(InputSubsystem != nullptr, TEXT("Failed to get InputSubsystem"));
	InputSubsystem->AddMappingContext(DefaultMappingContext, 0);

	auto* const World = GetWorld();

	check(HUDClass);
	MainHUD = CreateWidget<UMainHUDWidget>(World, HUDClass);

	checkf(MainHUD, TEXT("Failed to create MainHUD widget"));
	MainHUD->AddToViewport();

	MainHUD->OnCollectedOrbsChanged(GetOrbsCollected());
	MainHUD->OnHealthChanged(Health);

	check(PauseHUDClass);
	PauseHUD = CreateWidget<UUserWidget>(World, PauseHUDClass);

	check(GameOverHUDClass);
	GameOverHUD = CreateWidget<UUserWidget>(World, GameOverHUDClass);
	
	check(GameWonHUDClass);
	GameWonHUD = CreateWidget<UUserWidget>(World, GameWonHUDClass);

	GetWorldTimerManager().SetTimer(InteractionTimerHandle, this,
	                                &AMyCharacter::UpdateInteractionPrompt,
	                                InteractionCheckInterval.count(), true);
}

void AMyCharacter::HandleLookAction(const FInputActionValue& LookAxis)
{
	const FVector2D Look = LookAxis.Get<FVector2D>();

	AddControllerYawInput(Look.X);
	AddControllerPitchInput(Look.Y);
}

void AMyCharacter::HandleMoveAction(const FInputActionValue& MoveAxis)
{
	const FVector2D Look = MoveAxis.Get<FVector2D>();

	AddMovementInput(GetActorRightVector(), Look.X);
	AddMovementInput(GetActorForwardVector(), Look.Y);
}

void AMyCharacter::HandleInteractAction()
{
	if (FocusedActor.IsValid())
	{
		IInteractable::Execute_Interact(FocusedActor.Get(), this);
	}
}

void AMyCharacter::CollectOrb() noexcept
{
	++OrbsCollected;
	MainHUD->OnCollectedOrbsChanged(OrbsCollected);
	MainHUD->ShowOrbCollectedTint();
}

void AMyCharacter::SetPauseHUD()
{
	UWidgetLayoutLibrary::RemoveAllWidgets(this);
	PauseHUD->AddToViewport();
}

void AMyCharacter::SetGameHUD()
{
	UWidgetLayoutLibrary::RemoveAllWidgets(this);
	MainHUD->AddToViewport();
}

void AMyCharacter::TriggerGameOver()
{
	SwitchToGameScreen(*GameOverHUD);
}

void AMyCharacter::TriggerGameWon()
{
	SwitchToGameScreen(*GameWonHUD);
}

void AMyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	GetWorldTimerManager().ClearAllTimersForObject(this);
}

void AMyCharacter::EndInvincibility() noexcept
{
	IsInvincible = false;
}

void AMyCharacter::UpdateInteractionPrompt()
{
	// Make a line-tracing call every 0.1 sec to see what we're looking at
	// And create a corresponding interaction prompt in the HUD
	// The course 'hard codes' the InputAction inside the Door blueprint
	// Possible to do in C++, but... kind of bad
	// Let's do it a bit better
	// RayTrace-ing to see what object we're trying to use
	// And see if it can be used!
	const FVector Start = CameraComponent->GetComponentLocation();
	const FVector End   = Start + CameraComponent->GetForwardVector() * 300.f;

	FCollisionQueryParams Params{};
	Params.AddIgnoredActor(this);
	if (UWorld* const World = GetWorld())
	{
		// World->LineTraceSingleByChannel()
		// LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility,Params)
		FTraceDelegate Delegate{};
		Delegate.BindUObject(this, &AMyCharacter::HandleLineTraceCheck);

		World->AsyncLineTraceByChannel(EAsyncTraceType::Single, Start, End,
		                               ECollisionChannel::ECC_Visibility, Params,
		                               FCollisionResponseParams{}, &Delegate);
	}
}

void AMyCharacter::HandleLineTraceCheck(const FTraceHandle& TraceHandle,
                                        FTraceDatum& TraceDatum)
{
	if (!TraceDatum.OutHits.IsEmpty())
	{
		const FHitResult& HitResult = TraceDatum.OutHits[0];
		// Check if object we have is Interactable
		// use the UNREAL Interface, NOT THE C++ one
		// Then use the generated C++ Execute_ function to call the method
		if (AActor* const ActorHit = HitResult.GetActor();
		    ActorHit && ActorHit->Implements<UInteractable>())
		{
			FocusedActor = ActorHit;
			const EInteractionResult Result =
			    IInteractable::Execute_TryInteraction(ActorHit, this);

			switch (Result)
			{
				case EInteractionResult::Success:
					MainHUD->ShowScreenMessage("Press F to open the door");
					break;
				case EInteractionResult::Failed:
					MainHUD->ShowScreenMessage(
					    "You must collect all 3 orbs to open this door");
					break;
			}
		}
		else
		{
			FocusedActor = nullptr;
		}
	}
}

void AMyCharacter::SwitchToGameScreen(UUserWidget& NewWidget)
{
	UWidgetLayoutLibrary::RemoveAllWidgets(this);
	NewWidget.AddToViewport();
	
	auto* const PlayerController = Cast<APlayerController>(GetController());
	check(PlayerController);
	
	FInputModeUIOnly InputMode{};
	InputMode.SetWidgetToFocus(NewWidget.TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	
	PlayerController->SetInputMode(InputMode);
	PlayerController->SetShowMouseCursor(true);
	PlayerController->SetPause(true);
}

float AMyCharacter::TakeDamage(const float Damage,
                               [[maybe_unused]] const FDamageEvent& DamageEvent,
                               [[maybe_unused]] AController* const EventInstigator,
                               [[maybe_unused]] AActor* const DamageCauser)
{
	// Grace period so we don't lose health 60 times in one second
	if (IsInvincible)
	{
		return 0.f;
	}

	IsInvincible = true;
	GetWorldTimerManager().SetTimer(InvincibilityTimerHandle, this,
	                                &AMyCharacter::EndInvincibility,
	                                InvincibilityDuration, false);

	Health -= Damage;
	if (Health <= HealthEpsilon)
	{
		TriggerGameOver();
	}
	MainHUD->ShowDamageTint();
	MainHUD->OnHealthChanged(Health);

	return Damage;
}