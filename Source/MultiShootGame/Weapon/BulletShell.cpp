// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletShell.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABulletShell::ABulletShell()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletShellComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletShellComponent"));
	RootComponent = BulletShellComponent;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(
		TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetAutoActivate(false);
}

void ABulletShell::DestroyBulletShell()
{
	Destroy();
}

void ABulletShell::ThrowBulletShell()
{
	ProjectileMovementComponent->Velocity = (GetActorRotation() + FRotator(0, 180.f, 0)).Vector() *
		UKismetMathLibrary::RandomFloatInRange(MinInitialSpeed, MaxInitialSpeed);
	ProjectileMovementComponent->Activate();

	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABulletShell::DestroyBulletShell, DestroyDelay);
}
