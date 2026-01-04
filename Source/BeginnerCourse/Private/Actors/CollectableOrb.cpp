#include "CollectableOrb.h"

#include "GameLogic/MyCharacter.h"

ACollectableOrb::ACollectableOrb()
{
	auto& StaticMeshComponent = *GetStaticMeshComponent();

	const static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh{ TEXT(
		"/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere") };
	if (SphereMesh.Succeeded())
	{
		StaticMeshComponent.SetStaticMesh(SphereMesh.Object);
	}

	const static ConstructorHelpers::FObjectFinder<UMaterial> OceanMaterial{ TEXT(
		"/Game/StarterContent/Materials/M_Water_Ocean.M_Water_Ocean") };
	if (OceanMaterial.Succeeded())
	{
		StaticMeshComponent.SetMaterial(0, OceanMaterial.Object);
	}

	SetActorScale3D(FVector{ 0.5f, 0.5f, 0.5f });

	StaticMeshComponent.SetGenerateOverlapEvents(true);
	StaticMeshComponent.SetCollisionProfileName(TEXT("OverlapAll"));
}

void ACollectableOrb::BeginPlay()
{
	Super::BeginPlay();

	GetStaticMeshComponent()->OnComponentBeginOverlap.AddDynamic(
	    this, &ACollectableOrb::OnBeginOverlap);
}

void ACollectableOrb::OnBeginOverlap(
    [[maybe_unused]] UPrimitiveComponent* const OverlappedComponent,
    AActor* const OtherActor,
    [[maybe_unused]] UPrimitiveComponent* const OtherComp,
    [[maybe_unused]] const int32 OtherBodyIndex,
    [[maybe_unused]] bool bFromSweep,
    [[maybe_unused]] const FHitResult& SweepResult)
{
	if (AMyCharacter* const MyCharacter = Cast<AMyCharacter>(OtherActor))
	{
		MyCharacter->CollectOrb();
		UE_LOG(LogTemp, Warning, TEXT("%d"), MyCharacter->GetOrbsCollected());
		Destroy();
	}
}