// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiShootGameProjectileBase.h"
#include "MatineeCameraShake.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "MultiShootGameGrenade.generated.h"

UCLASS()
class MULTISHOOTGAME_API AMultiShootGameGrenade : public AMultiShootGameProjectileBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMultiShootGameGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	UStaticMeshComponent* GrenadeComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	UParticleSystemComponent* ParticleSystemComponent;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	UParticleSystem* ExplosionParticleSystem;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	USoundCue* ExplosionSoundCue;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<UMatineeCameraShake> GrenadeCameraShakeClass;

	FTimerHandle TimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	float DamageRadius = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	float ExplodedDelay = 2.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	FRotator ThrowRotatorPlus = FRotator(20.f, 0, 0);

	UFUNCTION(NetMulticast, Reliable)
	void ThrowGrenade_Multicast();

	void Explode();

	UFUNCTION(NetMulticast, Reliable)
	void Explode_Multicast();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Server, Reliable)
	void ThrowGrenade_Server(FRotator Direction, bool MultiThrow);

	virtual void ProjectileInitialize(float Damage) override;
};
