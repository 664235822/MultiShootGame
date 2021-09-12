// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiShootGameWeapon.h"
#include "MultiShootGame.h"
#include "MultiShootGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

// Sets default values
AMultiShootGameWeapon::AMultiShootGameWeapon()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponmeshComponent"));
	WeaponMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMultiShootGameWeapon::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenShots = 60.0f / RateOfFire;
}

void AMultiShootGameWeapon::Fire()
{
	AMultiShootGameCharacter* MyOwner = Cast<AMultiShootGameCharacter>(GetOwner());
	if (MyOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;

		if (MyOwner->bAimed)
		{
			EyeLocation = MyOwner->CurrentFPSCamera->CameraComponent->GetComponentLocation();
			EyeRotation = MyOwner->CurrentFPSCamera->CameraComponent->GetComponentRotation();
		}
		else
		{
			EyeLocation = MyOwner->CameraComponent->GetComponentLocation();
			EyeRotation = MyOwner->CameraComponent->GetComponentRotation();
		}

		FVector ShotDirection = EyeRotation.Vector();

		const float HalfRad = FMath::DegreesToRadians(BulletSpread);
		ShotDirection = FMath::VRandCone(ShotDirection, HalfRad, HalfRad);

		FVector TraceEnd = EyeLocation + (ShotDirection * 3000.f);

		FCollisionQueryParams QueryOParams;
		QueryOParams.AddIgnoredActor(MyOwner);
		QueryOParams.AddIgnoredActor(this);
		QueryOParams.bTraceComplex = true;
		QueryOParams.bReturnPhysicalMaterial = true;

		FVector TraceEndPoint = TraceEnd;

		EPhysicalSurface SurfaceType = SurfaceType_Default;

		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd,COLLISION_WEAPON, QueryOParams))
		{
			AActor* HitActor = Hit.GetActor();

			//SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

			float CurrentDamage = BaseDamage;

			//if (SurfaceType == SURFACE_FLESHVULNERABLE)
			//{
			//	CurrentDamage *= 2.5f;
			//}

			UGameplayStatics::ApplyPointDamage(HitActor, CurrentDamage, ShotDirection, Hit,
			                                   MyOwner->GetInstigatorController(),
			                                   MyOwner, DamageType);

			PlayImpactEffect(SurfaceType, Hit.ImpactPoint);

			TraceEndPoint = Hit.ImpactPoint;
		}

		PlayFireEffect(TraceEndPoint);

		LastFireTime = GetWorld()->TimeSeconds;
	}
}

void AMultiShootGameWeapon::PlayFireEffect(FVector TraceEndPoint)
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, WeaponMeshComponent, MuzzleSocketName);
	}

	APawn* MyOwner = Cast<APawn>(GetOwner());
	if (MyOwner)
	{
		APlayerController* PlayerController = Cast<APlayerController>(MyOwner->GetController());
		if (PlayerController)
		{
			PlayerController->ClientPlayCameraShake(FireCameraShake);
		}
	}
}

void AMultiShootGameWeapon::PlayImpactEffect(EPhysicalSurface SurfaceType, FVector ImpactPoint)
{
	UParticleSystem* SelectEffect = nullptr;
	switch (SurfaceType)
	{
	case SURFACE_FLESHDEFAULT:
	case SURFACE_FLESHVULNERABLE:
		SelectEffect = FleshImpactEffect;
		break;
	default:
		SelectEffect = DefaultImpactEffect;
		break;
	}

	if (SelectEffect)
	{
		const FVector MuzzleLocation = WeaponMeshComponent->GetSocketLocation(MuzzleSocketName);

		FVector TraceDirection = ImpactPoint - MuzzleLocation;
		TraceDirection.Normalize();

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectEffect, ImpactPoint,
		                                         TraceDirection.Rotation());
	}
}

void AMultiShootGameWeapon::StartFire()
{
	const float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMultiShootGameWeapon::Fire, TimeBetweenShots, true,
	                                FirstDelay);
}

void AMultiShootGameWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle);
}
