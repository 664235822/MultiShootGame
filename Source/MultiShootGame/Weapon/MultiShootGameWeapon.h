// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MatineeCameraShake.h"
#include "GameFramework/Pawn.h"
#include "Sound/SoundCue.h"
#include "MultiShootGameWeapon.generated.h"

UCLASS()
class MULTISHOOTGAME_API AMultiShootGameWeapon : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMultiShootGameWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void PlayFireEffect(FVector TraceEndPoint);

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Components")
	USkeletalMeshComponent* WeaponMeshComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName = "Muzzle";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName TracerTargetName = "Target";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AActor> TracerEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UMatineeCameraShake> FireCameraShake;

	FTimerHandle TimerHandle;

	float LastFireTime;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float RateOfFire = 600.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float DelayOfShotgun = 0.3f;
	
	float TimeBetweenShots;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (ClampMin = 0.0f))
	float BulletSpread = 1.f;
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* AudioComponent;
	
	void Fire();

	void StartFire();

	void StopFire();

	void ShotgunFire();
	
};
