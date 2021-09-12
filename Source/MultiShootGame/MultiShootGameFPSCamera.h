// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiShootGameWeapon.h"
#include "Camera/CameraComponent.h"
#include "MultiShootGameFPSCamera.generated.h"

/**
 * 
 */
UCLASS()
class MULTISHOOTGAME_API AMultiShootGameFPSCamera : public AMultiShootGameWeapon
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMultiShootGameFPSCamera();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	USkeletalMesh* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	USkeletalMesh* ShotgunMesh;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UCameraComponent* CameraComponent;
	
	void ToggleWeapon(bool Weapon);
};
