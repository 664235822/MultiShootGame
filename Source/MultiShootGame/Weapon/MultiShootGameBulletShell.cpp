// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiShootGameBulletShell.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMultiShootGameBulletShell::AMultiShootGameBulletShell()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletShellComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletShellComponent"));
	RootComponent = BulletShellComponent;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(
		TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetAutoActivate(false);
}

void AMultiShootGameBulletShell::DestroyBulletShell()
{
	Destroy();
}

void AMultiShootGameBulletShell::ThrowBulletShell()
{
	ProjectileMovementComponent->Velocity = (GetActorRotation() + ThrowDirection).Vector() *
		UKismetMathLibrary::RandomFloatInRange(MinInitialSpeed, MaxInitialSpeed);
	ProjectileMovementComponent->Activate();

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMultiShootGameBulletShell::DestroyBulletShell, DestroyDelay);
}
