// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiShootGameProjectile.h"
#include "../MultiShootGame.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

AMultiShootGameProjectile::AMultiShootGameProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitBoxExtent(FVector(70.f,5.f,5.f));
	CollisionComponent->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComponent->OnComponentHit.AddDynamic(this, &AMultiShootGameProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComponent->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComponent;

	ParticleSystemComponent=CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleSystemComponent->SetupAttachment(CollisionComponent);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	ProjectileMovement->InitialSpeed = 4000.f;
	ProjectileMovement->MaxSpeed = 4000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AMultiShootGameProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	EPhysicalSurface SurfaceType = SurfaceType_Default;

	if (OtherActor != nullptr) {
		
		SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

		float CurrentDamage = BaseDamage;

		if (SurfaceType == SURFACE_HEAD)
		{
			CurrentDamage *= 2.5f;
		}

		UGameplayStatics::ApplyPointDamage(OtherActor, CurrentDamage, GetActorRotation().Vector(), Hit,
										GetOwner()->GetInstigatorController(),
										GetOwner(), DamageType);
		
	}

	PlayImpactEffect(SurfaceType, Hit.ImpactPoint);

	Destroy();
}

void AMultiShootGameProjectile::PlayImpactEffect(EPhysicalSurface SurfaceType, FVector ImpactPoint)
{
	UParticleSystem* SelectEffect = nullptr;
	switch (SurfaceType)
	{
	case SURFACE_HEAD:
	case SURFACE_BODY:
		SelectEffect = FleshImpactEffect;
		break;
	default:
		SelectEffect = DefaultImpactEffect;
		break;
	}

	if (SelectEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectEffect, ImpactPoint,
												GetActorRotation());
	}
}