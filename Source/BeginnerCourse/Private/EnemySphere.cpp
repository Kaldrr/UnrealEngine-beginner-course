// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySphere.h"


namespace
{
	const FTransform SphereMeshTransform = []
	{
		FTransform transform{};
		transform.SetLocation({0.0, 0.0, -50.0});
		return transform;
	}();
}

// Sets default values
AEnemySphere::AEnemySphere()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	SphereMesh->SetupAttachment(Root);
	SphereMesh->SetComponentToWorld(SphereMeshTransform);

	FireParticleSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FireParticleSystem"));
	FireParticleSystem->SetupAttachment(SphereMesh);

	MovementComponent = CreateDefaultSubobject<UMovementPatrolComponent>(TEXT("MovementComponent"));
	MovementComponent->TargetComponent = SphereMesh;
}

// Called when the game starts or when spawned
void AEnemySphere::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemySphere::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}
