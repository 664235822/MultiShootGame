// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BulletShell.h"
#include "MatineeCameraShake.h"
#include "MultiShootGameClip.h"
#include "GameFramework/Pawn.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* WeaponMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* AudioComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName = "Muzzle";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName TracerTargetName = "Target";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName BulletShellName = "BulletShell";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName ClipBoneName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName ClipSocketName = "Clip";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	TSubclassOf<AActor> TracerEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<ABulletShell> BulletShellClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CameraShake")
	TSubclassOf<UMatineeCameraShake> FireCameraShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AMultiShootGameClip> ClipClass;

	FTimerHandle TimerHandle;

	float LastFireTime;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float RateOfFire = 600.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float DelayOfShotgun = 0.3f;

	float TimeBetweenShots;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (ClampMin = 0.0f))
	float BulletSpread = 1.f;

	virtual void ShakeCamera();

	AMultiShootGameClip* CurrentClip;

public:
	void Initialize();

	void Fire();

	void StartFire();

	void StopFire(bool StopAudio);

	void ShotgunFire();

	void EnablePhysicsSimulate();

	void ReloadAttachToPlayer(USkeletalMeshComponent* PlayerMesh, FName PlayerSocket);
	
	void ReloadAttachBack();
};
