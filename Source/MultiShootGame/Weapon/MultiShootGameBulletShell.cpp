// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiShootGameBulletShell.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMultiShootGameBulletShell::AMultiShootGameBulletShell()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	SetReplicateMovement(true);

	BulletShellComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletShellComponent"));
	BulletShellComponent->SetIsReplicated(true);
	RootComponent = BulletShellComponent;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(
		TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetAutoActivate(false);
	ProjectileMovementComponent->SetIsReplicated(true);
}

void AMultiShootGameBulletShell::ThrowBulletShell_Server_Implementation()
{
	ProjectileMovementComponent->Velocity = (GetActorRotation() + ThrowDirection).Vector() *
		UKismetMathLibrary::RandomFloatInRange(MinInitialSpeed, MaxInitialSpeed);

	ThrowBulletShell_Multicast();

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMultiShootGameBulletShell::DestroyBulletShell_Server,
	                                DestroyDelay);
}

void AMultiShootGameBulletShell::ThrowBulletShell_Multicast_Implementation()
{
	ProjectileMovementComponent->Activate();
}

void AMultiShootGameBulletShell::DestroyBulletShell_Server_Implementation()
{
	Destroy();
}
