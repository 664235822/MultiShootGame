// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiSHootGameEnemyWeapon.h"
#include "DrawDebugHelpers.h"
#include "Components/AudioComponent.h"
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

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->SetAutoActivate(false);
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
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(GetOwner());
		FHitResult HitResult;
		if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), EyeLocation, TraceEnd,TraceType_EnemyWeaponTrace, false,
		                                          IgnoreActors, EDrawDebugTrace::None, HitResult, true))
		{
			AActor* HitActor = HitResult.GetActor();

			SurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());

			float CurrentDamage = BaseDamage;

			if (SurfaceType == SURFACE_HEAD)
			{
				CurrentDamage *= 2.5f;
			}

			UGameplayStatics::ApplyPointDamage(HitActor, CurrentDamage, ShotDirection, HitResult,
			                                   MyOwner->GetInstigatorController(),
			                                   MyOwner, DamageType);

			TraceEndPoint = HitResult.ImpactPoint;
		}

		PlayFireEffect(TraceEndPoint);

		LastFireTime = GetWorld()->TimeSeconds;

		AudioComponent->Play();
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
	StopFire();
	WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	WeaponMeshComponent->SetSimulatePhysics(true);

	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AMultiShootGameEnemyWeapon::DestroyWeapon, DestroyDelay);
}
