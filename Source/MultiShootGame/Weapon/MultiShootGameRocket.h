// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MatineeCameraShake.h"
#include "MultiShootGameProjectileBase.h"
#include "Components/BoxComponent.h"
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

	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	UStaticMeshComponent* RocketComponent;

	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	UParticleSystemComponent* ParticleSystemComponent;

	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="Rocket")
	UParticleSystem* ExplosionParticleSystem;

	UPROPERTY(EditDefaultsOnly, Category="Rocket")
	USoundCue* ExplosionSoundCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Rocket")
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Rocket")
	TSubclassOf<UMatineeCameraShake> RocketCameraShakeClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Grenade")
	float DamageRadius = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Grenade")
	float ExplodedDelay = 2.0f;

	FTimerHandle TimerHandle;

	virtual void ProjectileInitialize(float Damage) override;
	
	void Explode();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
			   const FHitResult& Hit);
	
};
