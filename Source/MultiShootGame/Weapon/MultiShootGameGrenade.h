// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	UPROPERTY(VisibleDefaultsOnly, Category="Grenade")
	UStaticMeshComponent* GrenadeComponent;

	UPROPERTY(VisibleDefaultsOnly, Category="Grenade")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, Category="Grenade")
	UParticleSystem* ExplosionParticleSystem;

	UPROPERTY(EditDefaultsOnly, Category="Grenade")
	USoundCue* ExplosionSoundCue;

	FTimerHandle TimerHandle;

	void Explode();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Grenade")
	float ExplodedDelay = 4.0f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ThrowGrenade(FRotator Direction);
};
