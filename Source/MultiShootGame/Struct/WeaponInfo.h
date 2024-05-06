// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Weapon/MultiShootGameProjectileBase.h"
#include "Engine/DataTable.h"
#include "Sound/SoundCue.h"
#include "WeaponInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct MULTISHOOTGAME_API FWeaponInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponInfo)
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponInfo)
	USkeletalMesh* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponInfo)
	UTexture* WeaponImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponInfo)
	UStaticMesh* MagazineClipMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponInfo)
	USoundCue* FireSoundCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character)
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponInfo)
	TSubclassOf<AMultiShootGameProjectileBase> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponInfo)
	TSubclassOf<UCameraShakeBase> FireCameraShake;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponInfo)
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponInfo)
	float RateOfFire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponInfo)
	float DelayOfShotgun;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponInfo)
	float BulletSpread;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponInfo)
	float CameraSpread;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponInfo)
	float ZoomedFOV;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponInfo)
	FVector AimVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponInfo)
	bool SniperAim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponInfo)
	int BulletNumber;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponInfo)
	int FillUpBulletNumber;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponInfo)
	int MaxBulletNumber;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponInfo)
	int FillUpMaxBulletNumber;
};
