// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiShootGameClip.h"

// Sets default values
AMultiShootGameClip::AMultiShootGameClip()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

	ClipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClipMeshComponent"));
	RootComponent = ClipMeshComponent;
	ClipMeshComponent->SetCastHiddenShadow(true);
}

// Called when the game starts or when spawned
void AMultiShootGameClip::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMultiShootGameClip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
