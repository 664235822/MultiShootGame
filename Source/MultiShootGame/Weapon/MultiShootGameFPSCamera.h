// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EWeapon.h"
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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	USkeletalMesh* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	USkeletalMesh* ShotgunMesh;

	bool bWantToZoom = false;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float ZoomedFOV = 65.0f;

	float DefaultFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1f, ClampMax = 100.0f))
	float ZoomInterpSpeed = 20.0f;;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;

	void ToggleWeapon(EWeaponMode WeaponMode);

	void SetZoomed(bool WantToZoom);
};
