// Fill out your copyright notice in the Description page of Project Settings.


#include "Magazine.h"

// Sets default values
AMagazine::AMagazine()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MagazineMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MagazineMeshComponent"));
	RootComponent = MagazineMeshComponent;
}

// Called when the game starts or when spawned
void AMagazine::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMagazine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMagazine::ThrowMagazine(UStaticMesh* MagazineMesh)
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	MagazineMeshComponent->SetStaticMesh(MagazineMesh);
	MagazineMeshComponent->SetSimulatePhysics(true);

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMagazine::DestroyMagazine, DestroyDelay);
}

void AMagazine::DestroyMagazine()
{
	Destroy();
}
