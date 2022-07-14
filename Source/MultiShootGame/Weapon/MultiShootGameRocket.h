// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MatineeCameraShake.h"
#include "MultiShootGameProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Sound/SoundCue.h"
#include "MultiShootGameRocket.generated.h"

/**
 * 
 */
UCLASS()
class MULTISHOOTGAME_API AMultiShootGameRocket : public AMultiShootGameProjectileBase
{
	GENERATED_BODY()

public:
	AMultiShootGameRocket();

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	UStaticMeshComponent* RocketComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	UParticleSystemComponent* ParticleSystemComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	UParticleSystem* ExplosionParticleSystem;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	USoundCue* ExplosionSoundCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<UMatineeCameraShake> RocketCameraShakeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	float DamageRadius = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	float ExplodedDelay = 2.0f;

	FTimerHandle TimerHandle;

	virtual void ProjectileInitialize(float Damage) override;

	void Explode();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	           const FHitResult& Hit);
};
