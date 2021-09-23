// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/EWeapon.h"
#include "Weapon/MultiShootGameFPSCamera.h"
#include "Weapon/MultiShootGameWeapon.h"
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

	void BeginReload();

	UFUNCTION(BlueprintCallable)
	void EndReload();

	void ToggleWeapon();

	void ToggleSniper();

	void ToggleShotgun();

	UFUNCTION(BlueprintCallable)
	void ToggleWeaponBegin();

	UFUNCTION(BlueprintCallable)
	void ToggleWeaponEnd();

	UFUNCTION(BlueprintCallable)
	void Death();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* WeaponSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SniperSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* ShotgunSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* AudioComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AMultiShootGameWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AMultiShootGameWeapon> SniperClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AMultiShootGameWeapon> ShotgunClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponSocketName = "WeaponSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName BackWeaponSocketName = "BackWeaponSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName SniperSocketName = "SniperSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName BackSniperSocketName = "BackSniperSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName ShotgunSocketName = "ShotgunSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName BackShotgunSocketName = "BackShotgunSocket";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* FPSCameraSceneComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AMultiShootGameFPSCamera> FPSCameraClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UMatineeCameraShake> FPSCameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* WeaponOutAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* ReloadingAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	USoundCue* WeaponFireCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	USoundCue* SniperFireCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	USoundCue* ShotgunFireCue;

	UPROPERTY(BlueprintReadOnly)
	AMultiShootGameWeapon* CurrentWeapon;

	UPROPERTY(BlueprintReadOnly)
	AMultiShootGameWeapon* CurrentSniper;

	UPROPERTY(BlueprintReadOnly)
	AMultiShootGameWeapon* CurrentShotgun;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	EWeaponMode WeaponMode = EWeaponMode::Weapon;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(BlueprintReadOnly)
	AMultiShootGameFPSCamera* CurrentFPSCamera;

	bool bAimed = false;

private:
	void AimLookAround();

	bool CheckStatus();

	void EndAction();

	bool bFired = false;

	bool bDied = false;

	bool bReloading = false;

	bool bToggleWeapon = false;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
