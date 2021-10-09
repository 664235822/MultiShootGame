// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UMaterial* BloodDecalMaterial;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	           const FHitResult& Hit);

	void PlayImpactEffect(EPhysicalSurface SurfaceType, FVector ImpactPoint);

	UFUNCTION()
	void OnParticleCollide(FName EventName, float EmitterTime, int32 ParticleTime, FVector Location, FVector Velocity,
	                       FVector Direction, FVector Normal, FName BoneName, UPhysicalMaterial* PhysMat);
};
