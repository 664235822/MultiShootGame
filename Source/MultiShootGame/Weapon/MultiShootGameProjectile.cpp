// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiShootGameProjectile.h"
#include "MultiShootGameWeapon.h"
#include "MultiShootGame/MultiShootGame.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MultiShootGame/Character/MultiShootGameCharacter.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

AMultiShootGameProjectile::AMultiShootGameProjectile()
{
	// Use a sphere as a simple collision representation
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitBoxExtent(FVector(5.f, 5.f, 5.f));
	CollisionComponent->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComponent->SetIsReplicated(true);
	if (GetLocalRole() == ROLE_Authority)
	{
		CollisionComponent->OnComponentHit.AddDynamic(this, &AMultiShootGameProjectile::OnHit);
	}
	// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComponent->CanCharacterStepUpOn = ECB_No;
	CollisionComponent->bReturnMaterialOnMove = true;

	// Set as root component
	RootComponent = CollisionComponent;

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleSystemComponent->SetupAttachment(CollisionComponent);

	HitEffectComponent = CreateDefaultSubobject<UHitEffectComponent>(TEXT("HitEffectComponent"));

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	ProjectileMovement->InitialSpeed = 4000.f;
	ProjectileMovement->MaxSpeed = 4000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0;
	ProjectileMovement->SetIsReplicated(true);

	// Die after 3 seconds by default
	InitialLifeSpan = 4.0f;
}

void AMultiShootGameProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                      FVector NormalImpulse, const FHitResult& Hit)
{
	const EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

	if (Cast<ACharacter>(OtherActor))
	{
		if (SurfaceType == SURFACE_HEAD)
		{
			BaseDamage *= 2.5f;

			Cast<UHealthComponent>(OtherActor->GetComponentByClass(UHealthComponent::StaticClass()))->OnHeadShot.
				Broadcast();
		}

		UGameplayStatics::ApplyPointDamage(OtherActor, BaseDamage, GetActorRotation().Vector(), Hit,
		                                   GetOwner()->GetInstigatorController(), GetOwner(), DamageTypeClass);
	}
	else
	{
		UGameplayStatics::SpawnDecalAttached(BulletDecalMaterial, BulletDecalSize, OtherComp, NAME_None, Hit.Location,
		                                     Hit.ImpactNormal.Rotation(), EAttachLocation::KeepWorldPosition, 10.f);
	}

	HitEffectComponent->PlayHitEffect(SurfaceType, Hit.Location, GetActorRotation());

	Destroy();
}
