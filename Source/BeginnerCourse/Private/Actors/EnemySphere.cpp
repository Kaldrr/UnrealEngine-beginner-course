#include "EnemySphere.h"

#include "Components/AudioComponent.h"
#include "GameLogic/MyCharacter.h"
#include "Kismet/GameplayStatics.h"

namespace
{
const FTransform SphereMeshTransform = [] {
	FTransform Transform{};
	Transform.SetLocation({ 0.0, 0.0, -50.0 });
	return Transform;
}();
} // namespace

AEnemySphere::AEnemySphere()
{
	// Set this actor to call Tick() every frame.
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root          = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	SphereMesh->SetupAttachment(Root);
	SphereMesh->SetComponentToWorld(SphereMeshTransform);

	SphereMesh->SetGenerateOverlapEvents(true);
	SphereMesh->SetCollisionProfileName(TEXT("OverlapAll"));

	FireParticleSystem =
	    CreateDefaultSubobject<UNiagaraComponent>(TEXT("FireParticleSystem"));
	FireParticleSystem->SetupAttachment(SphereMesh);

	MovementComponent =
	    CreateDefaultSubobject<UMovementPatrolComponent>(TEXT("MovementComponent"));
	MovementComponent->TargetComponent = SphereMesh;

	FireAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FireSound"));
	FireAudioComponent->SetupAttachment(SphereMesh);

	const static ConstructorHelpers::FObjectFinder<USoundBase> DefaultFireSound{
		TEXT("/Game/StarterContent/Audio/Fire01_Cue.Fire01_Cue")
	};
	if (DefaultFireSound.Succeeded())
	{
		FireSound = DefaultFireSound.Object;
	}
}

void AEnemySphere::BeginPlay()
{
	Super::BeginPlay();

	SphereMesh->OnComponentBeginOverlap.AddDynamic(this,
	                                               &AEnemySphere::OnBeginOverlap);

	check(FireAudioComponent) check(FireSound);
	FireAudioComponent->SetSound(FireSound);
	FireAudioComponent->Play();
}

void AEnemySphere::OnBeginOverlap(
    [[maybe_unused]] UPrimitiveComponent* const OverlappedComponent,
    AActor* const OtherActor,
    [[maybe_unused]] UPrimitiveComponent* const OtherComp,
    [[maybe_unused]] const int32 OtherBodyIndex,
    [[maybe_unused]] const bool bFromSweep,
    [[maybe_unused]] const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AMyCharacter>())
	{
		UGameplayStatics::ApplyDamage(OtherActor, 0.2f, GetInstigatorController(),
		                              this, UDamageType::StaticClass());
	}
}
