// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MultiShootGameFPSCamera.h"
#include "MultiShootGameWeapon.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "MatineeCameraShake.h"
#include "Sound/SoundCue.h"
#include "MultiShootGameCharacter.generated.h"

UCLASS(config=Game)
class AMultiShootGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMultiShootGameCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void StartFire();

	void StopFire();

	void MoveForward(float Value);

	void MoveRight(float Value);

	void BeginFastRun();

	void EndFastRun();

	void BeginCrouch();

	void EndCrouch();

	void ToggleCrouch();

	void BeginAim();

	void EndAim();

	virtual void Jump() override;

	virtual void StopJumping() override;

	UFUNCTION(BlueprintCallable)
	void Footstep();

	UFUNCTION(BlueprintCallable)
	void Death();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AMultiShootGameWeapon> WeaponClass;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	FName WeaponSocketName = "WeaponSocket";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* FPSCameraSceneComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AMultiShootGameFPSCamera> FPSCameraClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UMatineeCameraShake> FPSCameraShakeClass;

	UPROPERTY(BlueprintReadOnly)
	AMultiShootGameWeapon* CurrentWeapon;

	UPROPERTY(BlueprintReadOnly)
	AMultiShootGameFPSCamera* CurrentFPSCamera;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	UAnimMontage* JumpAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	UAnimMontage* AimAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	UAnimMontage* CrouchAimAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	USoundCue* FootstepCue;

private:
	void AimLookAround();

	bool bFired = false;

	bool bAimed = false;

	bool bDied = false;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
