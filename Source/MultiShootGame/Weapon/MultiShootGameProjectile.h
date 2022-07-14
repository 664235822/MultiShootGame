// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MultiShootGameProjectileBase.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MultiShootGame/Component/HitEffectComponent.h"
#include "MultiShootGameProjectile.generated.h"

UCLASS(config=Game)
class AMultiShootGameProjectile : public AMultiShootGameProjectileBase
{
	GENERATED_BODY()

public:
	AMultiShootGameProjectile();

protected:
	
	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	UBoxComponent* CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	UParticleSystemComponent* ParticleSystemComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	UHitEffectComponent* HitEffectComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	UMaterialInterface* BulletDecalMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	FVector BulletDecalSize;
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	           const FHitResult& Hit);
	
};
