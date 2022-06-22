// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiShootGamePlayGameCharacter.h"
#include "Components/SceneCaptureComponent2D.h"

// Sets default values
AMultiShootGamePlayGameCharacter::AMultiShootGamePlayGameCharacter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMeshComponent"));
	RootComponent = CharacterMeshComponent;

	SceneCaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2D"));
	SceneCaptureComponent2D->SetupAttachment(CharacterMeshComponent);

	SpotLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLightComponent"));
	SpotLightComponent->SetupAttachment(CharacterMeshComponent);
}

// Called when the game starts or when spawned
void AMultiShootGamePlayGameCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMultiShootGamePlayGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
