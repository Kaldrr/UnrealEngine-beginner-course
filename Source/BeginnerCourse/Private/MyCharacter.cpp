// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interactable.h"

AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Add camera as a component in C++
	// But leave editing its properties to a blueprint subclass
	// Much easier iteration when we will mess around with the camera settings
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(RootComponent);
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	auto* const PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("AMyCharacter: Unexpected controller class"));
		return;
	}
	
	if (auto* const LocalPlayer = PlayerController->GetLocalPlayer();
		LocalPlayer)
	{
		if (auto* const InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
			InputSubsystem)
		{
			InputSubsystem->AddMappingContext(DefaultMappingContext, 0);			
		}
	}
}

void AMyCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
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

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(
		Hit, Start, End, ECC_Visibility, Params))
	{
		if (AActor* const HitActor = Hit.GetActor();
			HitActor)
		{
			// Check if object we have is Interactable
			// use the UNREAL Interface, NOT THE C++ one
			// Then use the generated C++ Execute_ function to call the method
			if (HitActor->Implements<UInteractable>())
			{
				IInteractable::Execute_Interact(HitActor);
			}
		}
	}
}
