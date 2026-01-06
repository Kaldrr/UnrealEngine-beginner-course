#include "MyCharacter.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameLogic/MyGameMode.h"
#include "Interfaces/Interactable.h"

namespace
{
constexpr float HealthEpsilon = 0.001f;
}

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

	const auto* const PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("AMyCharacter: Unexpected controller class"));
		return;
	}

	if (const auto* const LocalPlayer = PlayerController->GetLocalPlayer())
	{
		if (auto* const InputSubsystem =
		        LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			InputSubsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (HUDClass)
	{
		if (MainHUD = CreateWidget<UMainHUDWidget>(GetWorld(), HUDClass);
			MainHUD != nullptr)
		{
			MainHUD->AddToViewport();
			
			MainHUD->OnCollectedOrbsChanged(GetOrbsCollected());
			MainHUD->OnHealthChanged(Health);
		}
	}
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
	// The course 'hard codes' the InputAction inside the Door blueprint
	// Possible to do in C++, but... kind of bad
	// Let's do it a bit better
	// RayTrace-ing to see what object we're trying to use
	// And see if it can be used!
	const FVector Start = CameraComponent->GetComponentLocation();
	const FVector End   = Start + CameraComponent->GetForwardVector() * 300.f;

	FHitResult Hit{};
	FCollisionQueryParams Params{};
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility,
	                                         Params))
	{
		if (AActor* const HitActor = Hit.GetActor())
		{
			// Check if object we have is Interactable
			// use the UNREAL Interface, NOT THE C++ one
			// Then use the generated C++ Execute_ function to call the method
			if (HitActor->Implements<UInteractable>())
			{
				IInteractable::Execute_Interact(HitActor, this);
			}
		}
	}
}
void AMyCharacter::CollectOrb() noexcept
{
	++OrbsCollected;
	MainHUD->OnCollectedOrbsChanged(OrbsCollected);
	MainHUD->ShowOrbCollectedTint();
}

void AMyCharacter::EndInvincibility() noexcept
{
	IsInvincible = false;
}

float AMyCharacter::TakeDamage(const float Damage,
                               const FDamageEvent& DamageEvent,
                               AController* const EventInstigator,
                               AActor* const DamageCauser)
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
		if (const UWorld* const World = GetWorld())
		{
			if (const AMyGameMode* const GameMode =
			        World->GetAuthGameMode<AMyGameMode>())
			{
				GameMode->TriggerGameOver();
			}
		}
	}
	MainHUD->ShowDamageTint();
	MainHUD->OnHealthChanged(Health);

	return Damage;
}