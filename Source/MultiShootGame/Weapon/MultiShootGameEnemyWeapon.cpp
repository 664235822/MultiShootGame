// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiSHootGameEnemyWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Engine/Public/TimerManager.h"
#include "MultiShootGame/MultiShootGame.h"

// Sets default values
AMultiShootGameEnemyWeapon::AMultiShootGameEnemyWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
	RootComponent = WeaponMeshComponent;
}

// Called when the game starts or when spawned
void AMultiShootGameEnemyWeapon::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenShots = 60.0f / RateOfFire;
}

void AMultiShootGameEnemyWeapon::Fire()
{
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();

		float HalfRad = FMath::DegreesToRadians(BulletSpread);
		ShotDirection = FMath::VRandCone(ShotDirection, HalfRad, HalfRad);

		FVector TraceEnd = EyeLocation + (ShotDirection * 10000);

		FCollisionQueryParams QueryOParams;
		QueryOParams.AddIgnoredActor(MyOwner);
		QueryOParams.AddIgnoredActor(this);
		QueryOParams.bTraceComplex = true;
		QueryOParams.bReturnPhysicalMaterial = true;

		FVector TraceEndPoint = TraceEnd;

		EPhysicalSurface SurfaceType = SurfaceType_Default;

		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd,ECC_Weapon, QueryOParams))
		{
			AActor* HitActor = Hit.GetActor();

			SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

			float CurrentDamage = BaseDamage;

			if (SurfaceType == SURFACE_HEAD)
			{
				CurrentDamage *= 2.5f;
			}

			UGameplayStatics::ApplyPointDamage(HitActor, CurrentDamage, ShotDirection, Hit,
			                                   MyOwner->GetInstigatorController(),
			                                   MyOwner, DamageType);

			TraceEndPoint = Hit.ImpactPoint;
		}

		PlayFireEffect(TraceEndPoint);

		LastFireTime = GetWorld()->TimeSeconds;
	}
}

void AMultiShootGameEnemyWeapon::PlayFireEffect(FVector TraceEndPoint)
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, WeaponMeshComponent, MuzzleSocketName);
	}

	if (TracerEffect)
	{
		const FVector MuzzleLocation = WeaponMeshComponent->GetSocketLocation(MuzzleSocketName);

		UParticleSystemComponent* TracerComponent = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(), TracerEffect, MuzzleLocation);
		if (TracerComponent)
		{
			TracerComponent->SetVectorParameter(TracerTargetName, TraceEndPoint);
		}
	}
}

void AMultiShootGameEnemyWeapon::DestroyWeapon()
{
	Destroy();
}

void AMultiShootGameEnemyWeapon::StartFire()
{
	const float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMultiShootGameEnemyWeapon::Fire, TimeBetweenShots, true,
	                                FirstDelay);
}

void AMultiShootGameEnemyWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle);
}

void AMultiShootGameEnemyWeapon::EnablePhysicsSimulate()
{
	WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	WeaponMeshComponent->SetSimulatePhysics(true);

	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AMultiShootGameEnemyWeapon::DestroyWeapon, DestroyDelay);
}