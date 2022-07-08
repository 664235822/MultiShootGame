// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiShootGameShotgun.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "MultiShootGame/MultiShootGame.h"
#include "MultiShootGame/Character/MultiShootGameCharacter.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

AMultiShootGameShotgun::AMultiShootGameShotgun()
{
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>("RootSceneComponent");
	RootComponent = RootSceneComponent;

	// Use a sphere as a simple collision representation
	CollisionComponent1 = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent1"));
	CollisionComponent1->InitBoxExtent(FVector(5.f, 5.f, 5.f));
	CollisionComponent1->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComponent1->OnComponentHit.AddDynamic(this, &AMultiShootGameShotgun::OnHit);
	// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComponent1->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComponent1->CanCharacterStepUpOn = ECB_No;
	CollisionComponent1->bReturnMaterialOnMove = true;

	// Set as root component
	CollisionComponent1->SetupAttachment(RootComponent);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement1 = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileMovement1->UpdatedComponent = CollisionComponent1;
	ProjectileMovement1->InitialSpeed = 4000.f;
	ProjectileMovement1->MaxSpeed = 4000.f;
	ProjectileMovement1->bRotationFollowsVelocity = true;
	ProjectileMovement1->bShouldBounce = false;
	ProjectileMovement1->ProjectileGravityScale = 0;

	// Use a sphere as a simple collision representation
	CollisionComponent2 = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent2"));
	CollisionComponent2->InitBoxExtent(FVector(5.f, 5.f, 5.f));
	CollisionComponent2->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComponent2->OnComponentHit.AddDynamic(this, &AMultiShootGameShotgun::OnHit);
	// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComponent2->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComponent2->CanCharacterStepUpOn = ECB_No;
	CollisionComponent2->bReturnMaterialOnMove = true;

	// Set as root component
	CollisionComponent2->SetupAttachment(RootComponent);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement2 = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent2"));
	ProjectileMovement2->UpdatedComponent = CollisionComponent2;
	ProjectileMovement2->InitialSpeed = 4000.f;
	ProjectileMovement2->MaxSpeed = 4000.f;
	ProjectileMovement2->bRotationFollowsVelocity = true;
	ProjectileMovement2->bShouldBounce = false;
	ProjectileMovement2->ProjectileGravityScale = 0;

	// Use a sphere as a simple collision representation
	CollisionComponent3 = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent3"));
	CollisionComponent3->InitBoxExtent(FVector(5.f, 5.f, 5.f));
	CollisionComponent3->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComponent3->OnComponentHit.AddDynamic(this, &AMultiShootGameShotgun::OnHit);
	// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComponent3->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComponent3->CanCharacterStepUpOn = ECB_No;
	CollisionComponent3->bReturnMaterialOnMove = true;

	// Set as root component
	CollisionComponent3->SetupAttachment(RootComponent);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement3 = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent3"));
	ProjectileMovement3->UpdatedComponent = CollisionComponent3;
	ProjectileMovement3->InitialSpeed = 4000.f;
	ProjectileMovement3->MaxSpeed = 4000.f;
	ProjectileMovement3->bRotationFollowsVelocity = true;
	ProjectileMovement3->bShouldBounce = false;
	ProjectileMovement3->ProjectileGravityScale = 0;

	// Use a sphere as a simple collision representation
	CollisionComponent4 = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent4"));
	CollisionComponent4->InitBoxExtent(FVector(5.f, 5.f, 5.f));
	CollisionComponent4->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComponent4->OnComponentHit.AddDynamic(this, &AMultiShootGameShotgun::OnHit);
	// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComponent4->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComponent4->CanCharacterStepUpOn = ECB_No;
	CollisionComponent4->bReturnMaterialOnMove = true;

	// Set as root component
	CollisionComponent4->SetupAttachment(RootComponent);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement4 = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent4"));
	ProjectileMovement4->UpdatedComponent = CollisionComponent4;
	ProjectileMovement4->InitialSpeed = 4000.f;
	ProjectileMovement4->MaxSpeed = 4000.f;
	ProjectileMovement4->bRotationFollowsVelocity = true;
	ProjectileMovement4->bShouldBounce = false;
	ProjectileMovement4->ProjectileGravityScale = 0;

	HitEffectComponent = CreateDefaultSubobject<UHitEffectComponent>(TEXT("HitEffectComponent"));

	// Die after 1 seconds by default
	InitialLifeSpan = 1.0f;
}

void AMultiShootGameShotgun::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                   FVector NormalImpulse, const FHitResult& Hit)
{
	const EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

	if (Cast<ACharacter>(OtherActor))
	{
		if (SurfaceType == SURFACE_HEAD)
		{
			BaseDamage *= 2.5f;
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

	HitComp->DestroyComponent();
}
