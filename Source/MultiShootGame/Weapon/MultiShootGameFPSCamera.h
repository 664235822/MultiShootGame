// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EWeapon.h"
#include "MultiShootGameWeapon.h"
#include "Camera/CameraComponent.h"
#include "Sound/SoundCue.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	USkeletalMesh* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	USkeletalMesh* ShotgunMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	USoundCue* WeaponFireCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	USoundCue* SniperFireCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	USoundCue* ShotgunFireCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CameraShake")
	TSubclassOf<UMatineeCameraShake> SniperCameraShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camerashake")
	TSubclassOf<UMatineeCameraShake> ShotgunCameraShake;

	bool bWantToZoom = false;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float ZoomedFOV = 65.0f;

	float DefaultFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (ClampMin = 0.1f, ClampMax = 100.0f))
	float ZoomInterpSpeed = 20.0f;;

	virtual void ShakeCamera() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UCameraComponent* GetCameraComponent() const;

	void ToggleWeapon(EWeaponMode WeaponMode);

	void SetZoomed(bool WantToZoom);
};
