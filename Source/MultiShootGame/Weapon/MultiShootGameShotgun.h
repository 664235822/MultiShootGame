// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiShootGameProjectileBase.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MultiShootGame/Component/HitEffectComponent.h"
#include "MultiShootGameShotgun.generated.h"

/**
 * 
 */
UCLASS()
class MULTISHOOTGAME_API AMultiShootGameShotgun : public AMultiShootGameProjectileBase
{
	GENERATED_BODY()

public:

	AMultiShootGameShotgun();

protected:

	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	USceneComponent* RootSceneComponent;
	
	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	UBoxComponent* CollisionComponent1;

	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	UBoxComponent* CollisionComponent2;

	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	UBoxComponent* CollisionComponent3;

	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	UBoxComponent* CollisionComponent4;

	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	UHitEffectComponent* HitEffectComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement1;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement2;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement3;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	UMaterialInterface* BulletDecalMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	FVector BulletDecalSize;
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
			   const FHitResult& Hit);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
						int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
