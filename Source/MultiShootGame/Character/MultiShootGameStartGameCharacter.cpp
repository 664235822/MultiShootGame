// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiShootGameStartGameCharacter.h"
#include "Components/SceneCaptureComponent2D.h"

// Sets default values
AMultiShootGameStartGameCharacter::AMultiShootGameStartGameCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMeshComponent"));
	RootComponent = CharacterMeshComponent;

	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
	WeaponMeshComponent->SetupAttachment(CharacterMeshComponent, WeaponSocketName);

	SceneCaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2D"));
	SceneCaptureComponent2D->SetupAttachment(CharacterMeshComponent);

	SpotLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLightComponent"));
	SpotLightComponent->SetupAttachment(CharacterMeshComponent);
}

// Called when the game starts or when spawned
void AMultiShootGameStartGameCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMultiShootGameStartGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
