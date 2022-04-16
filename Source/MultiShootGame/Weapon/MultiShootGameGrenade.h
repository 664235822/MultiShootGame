// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MatineeCameraShake.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "MultiShootGameGrenade.generated.h"

UCLASS()
class MULTISHOOTGAME_API AMultiShootGameGrenade : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMultiShootGameGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	UStaticMeshComponent* GrenadeComponent;

	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	UParticleSystemComponent* ParticleSystemComponent;

	UPROPERTY(EditDefaultsOnly, Category="Grenade")
	UParticleSystem* ExplosionParticleSystem;

	UPROPERTY(EditDefaultsOnly, Category="Grenade")
	USoundCue* ExplosionSoundCue;

	UPROPERTY(EditDefaultsOnly, Category="Grenade")
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Grenade")
	TSubclassOf<UMatineeCameraShake> GrenadeCameraShakeClass;

	FTimerHandle TimerHandle;

	void Explode();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Grenade")
	float BaseDamage = 150.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Grenade")
	float DamageRadius = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Grenade")
	float ExplodedDelay = 4.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Grenade")
	FRotator ThrowRotatorPlus = FRotator(20.f, 0, 0);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ThrowGrenade(FRotator Direction, bool MultiPly);
};
