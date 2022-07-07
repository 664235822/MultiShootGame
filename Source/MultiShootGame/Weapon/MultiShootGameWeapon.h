// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BulletShell.h"
#include "Magazine.h"
#include "GameFramework/Pawn.h"
#include "MultiShootGame/Enum/EWeaponMode.h"
#include "MultiShootGame/Struct/WeaponInfo.h"
#include "Sound/SoundCue.h"
#include "MultiShootGameWeapon.generated.h"

class AMultiShootGameCharacter;

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

	virtual bool BulletCheck(AMultiShootGameCharacter* MyOwner);
	
	virtual void BulletFire(AMultiShootGameCharacter* MyOwner);

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
	EWeaponMode CurrentWeaponMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<ABulletShell> BulletShellClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AMagazine> MagazineClass;

	FTimerHandle TimerHandle;

	float LastFireTime;

	float TimeBetweenShots;

	bool Loop = false;

	void ShakeCamera();

public:
	void Fire();

	void StartFire();

	void StopFire();

	void FireOfDelay();
	
	void EnablePhysicsSimulate();

	void ReloadShowMagazine(bool Enabled);

	void BulletReload();
	
	USkeletalMeshComponent* GetWeaponMeshComponent() const;

	UAudioComponent* GetAudioComponent() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FWeaponInfo WeaponInfo;
};
