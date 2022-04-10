// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiShootGameStartGameCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneCaptureComponent2D.h"

// Sets default values
AMultiShootGameStartGameCharacter::AMultiShootGameStartGameCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneCaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2D"));
	SceneCaptureComponent2D->SetupAttachment(GetArrowComponent());

	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
	WeaponMeshComponent->SetupAttachment(GetMesh(), WeaponSocketName);
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

// Called to bind functionality to input
void AMultiShootGameStartGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
