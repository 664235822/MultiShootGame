// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiShootGameMagazineClip.h"

// Sets default values
AMultiShootGameMagazineClip::AMultiShootGameMagazineClip()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bOnlyRelevantToOwner = true;

	MagazineClipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MagazineMeshComponent"));
	RootComponent = MagazineClipMeshComponent;
}

// Called when the game starts or when spawned
void AMultiShootGameMagazineClip::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMultiShootGameMagazineClip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMultiShootGameMagazineClip::ThrowMagazineClip(UStaticMesh* MagazineClipMesh)
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	MagazineClipMeshComponent->SetStaticMesh(MagazineClipMesh);
	MagazineClipMeshComponent->SetSimulatePhysics(true);

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMultiShootGameMagazineClip::DestroyMagazineClip, DestroyDelay);
}

void AMultiShootGameMagazineClip::DestroyMagazineClip()
{
	Destroy();
}
