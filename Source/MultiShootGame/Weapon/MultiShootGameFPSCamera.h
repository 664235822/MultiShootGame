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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float ZoomedFOV = 65.0f;

	float DefaultFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (ClampMin = 0.1f, ClampMax = 100.0f))
	float ZoomInterpSpeed = 20.0f;;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UCameraComponent* GetCameraComponent() const;

	void SetWeaponInfo(const AMultiShootGameWeapon* Weapon);
};
