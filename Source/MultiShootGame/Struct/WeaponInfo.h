// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct MULTISHOOTGAME_API FWeaponInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WeaponInfo")
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WeaponInfo")
	USkeletalMesh* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WeaponInfo")
	UTexture* WeaponImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WeaponInfo")
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WeaponInfo")
	float RateOfFire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WeaponInfo")
	float DelayOfShotgun;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WeaponInfo")
	float BulletSpread;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WeaponInfo")
	float CameraSpread;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WeaponInfo")
	FVector AimVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WeaponInfo")
	bool AimTexture;
};
