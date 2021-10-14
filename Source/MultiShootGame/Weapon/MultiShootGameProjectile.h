// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ImpactParticleSystem.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MultiShootGameProjectile.generated.h"

UCLASS(config=Game)
class AMultiShootGameProjectile : public AActor
{
	GENERATED_BODY()

public:
	AMultiShootGameProjectile();

protected:
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	UBoxComponent* CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	UParticleSystemComponent* ParticleSystemComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float BaseDamage = 20.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Projectile)
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Projectile)
	TSubclassOf<AImpactParticleSystem> DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Projectile)
	TSubclassOf<AImpactParticleSystem> FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Projectile)
	TSubclassOf<AImpactParticleSystem> StoneImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Projectile)
	TSubclassOf<AImpactParticleSystem> WoodImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Projectile)
	UMaterial* BulletDecalMaterial;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	           const FHitResult& Hit);

	void PlayImpactEffect(EPhysicalSurface SurfaceType, FVector ImpactPoint);
};
