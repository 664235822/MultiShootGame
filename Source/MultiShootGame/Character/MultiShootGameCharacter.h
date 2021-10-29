// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HealthComponent.h"
#include "../Weapon/EWeapon.h"
#include "../Weapon/MultiShootGameFPSCamera.h"
#include "../Weapon/MultiShootGameWeapon.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "MatineeCameraShake.h"
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

	void BeginSniperReload();

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

	UFUNCTION()
	void OnHealthChanged(UHealthComponent* OwningHealthComponent, float Health, float HealthDelta,
	                     const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* FPSCameraSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* WeaponSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SniperSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* ShotgunSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComponent;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AMultiShootGameFPSCamera> FPSCameraClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UMatineeCameraShake> MovementCameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UMatineeCameraShake> FPSCameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* WeaponOutAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* ReloadAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* SniperReloadAnimMontage;

	UPROPERTY(BlueprintReadOnly)
	AMultiShootGameWeapon* CurrentWeapon;

	UPROPERTY(BlueprintReadOnly)
	AMultiShootGameWeapon* CurrentSniper;

	UPROPERTY(BlueprintReadOnly)
	AMultiShootGameWeapon* CurrentShotgun;
	
private:
	void AimLookAround();

	bool CheckStatus(bool checkAimed);

	void EndAction();

	bool bFired = false;

	bool bReloading = false;

	bool bToggleWeapon = false;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	USceneComponent* GetFPSCameraSceneComponent() const;

	UCameraComponent* GetCameraComponent() const;

	UPROPERTY(BlueprintReadOnly)
	AMultiShootGameFPSCamera* CurrentFPSCamera;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	EWeaponMode WeaponMode = EWeaponMode::Weapon;

	bool bAimed = false;
};
