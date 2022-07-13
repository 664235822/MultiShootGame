// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MatineeCameraShake.h"
#include "MultiShootGameEnemyWeapon.generated.h"

UCLASS()
class MULTISHOOTGAME_API AMultiShootGameEnemyWeapon : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AMultiShootGameEnemyWeapon();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void Fire();

    void PlayFireEffect(FVector TraceEndPoint);

    void DestroyWeapon();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USkeletalMeshComponent* WeaponMeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UAudioComponent* AudioComponent;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    TSubclassOf<UDamageType> DamageType;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    float BaseDamage = 5.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    float DestroyDelay = 10.0f;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    FName MuzzleSocketName = "Muzzle";

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    FName TracerTargetName = "Target";

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    UParticleSystem* MuzzleEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    UParticleSystem* TracerEffect;

    FTimerHandle TimerHandle;

    FTimerHandle DestroyTimerHandle;

    float LastFireTime;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    float RateOfFire = 600.0f;

    float TimeBetweenShots;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (ClampMin = 0.0f))
    float BulletSpread = 1.0f;

public:

    void StartFire();

    void StopFire();

    void EnablePhysicsSimulate();
};
