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
#include "MultiShootGameEnemyCharacter.h"
#include "MultiShootGame/Weapon/MultiShootGameGrenade.h"
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

	UFUNCTION(BlueprintCallable)
	void ReloadAttachToPlayer();

	UFUNCTION(BlueprintCallable)
	void ReloadAttachBack();

	void BeginThrowGrenade();

	UFUNCTION(BlueprintCallable)
	void EndThrowGrenade();

	void ThrowGrenade();

	UFUNCTION(BlueprintCallable)
	void ThrowGrenadeOut();

	UFUNCTION(BlueprintCallable)
	void SpawnGrenade();

	void KnifeAttack();

	UFUNCTION(BlueprintCallable)
	void BeginKnifeAttack();

	UFUNCTION(BlueprintCallable)
	void EndKnifeAttack();

	UFUNCTION(BlueprintCallable)
	void NextKnifeAttack();

	UFUNCTION(BlueprintCallable)
	void TakeDownAttack();

	void ToggleWeapon();

	void ToggleSniper();

	void ToggleShotgun();

	UFUNCTION(BlueprintCallable)
	void ToggleWeaponBegin();

	UFUNCTION(BlueprintCallable)
	void ToggleWeaponEnd();

	UFUNCTION(BlueprintCallable)
	void Hit();

	UFUNCTION(BlueprintCallable)
	void Death();

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
	USceneComponent* GrenadeSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* KnifeSkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* TakeDownKnifeSkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AMultiShootGameWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AMultiShootGameWeapon> SniperClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AMultiShootGameWeapon> ShotgunClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AMultiShootGameGrenade> GrenadeClass;

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

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName ClipPlayerSocketName = "ClipSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName GrenadeSocketName = "GrenadeSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName KnifeSocketName = "KnifeSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName TakeDownKnifeSocketName = "TakeDownKnifeSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AMultiShootGameFPSCamera> FPSCameraClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UMatineeCameraShake> MovementCameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UMatineeCameraShake> FPSCameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UMatineeCameraShake> KnifeCameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* WeaponOutAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* ReloadAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* SniperReloadAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* ThrowGrenadeAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* KnifeAttackAnimMontage[3];

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* TakeDownAttackerAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<UUserWidget> DefaultUserWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<UUserWidget> SniperUserWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	EWeaponMode WeaponMode = EWeaponMode::Weapon;

	UPROPERTY(BlueprintReadOnly)
	AMultiShootGameWeapon* CurrentWeapon;

	UPROPERTY(BlueprintReadOnly)
	AMultiShootGameWeapon* CurrentSniper;

	UPROPERTY(BlueprintReadOnly)
	AMultiShootGameWeapon* CurrentShotgun;

	UPROPERTY(BlueprintReadOnly)
	AMultiShootGameFPSCamera* CurrentFPSCamera;

	UPROPERTY(BlueprintReadOnly)
	AMultiShootGameGrenade* CurrentGrenade;

	UFUNCTION()
	void OnHealthChanged(UHealthComponent* OwningHealthComponent, float Health, float HealthDelta,
	                     const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void ToggleDefaultAimWidget(bool Enabled);

	void ToggleSniperAimWidget(bool Enabled);

	void ToggleUseControlRotation(bool Enabled);

	void AimLookAround();

	bool CheckStatus(bool CheckAimed, bool CheckThrowGrenade);

	void EndAction();

	bool bFired = false;

	bool bAimed = false;

	bool bReloading = false;

	bool bSniperReloading = false;

	bool bToggleWeapon = false;

	bool bBeginThrowGrenade = false;

	bool bThrowingGrenade = false;

	bool bSpawnGrenade = false;

	bool bKnifeAttack = false;

	bool bNextKnifeAttack = false;

	bool bTakeDown = false;

	bool bTakingDown = false;

	bool bUseControlRotation = false;

	bool bEnableMovement = true;

	int KnifeComboIndex = 0;

	FVector SpawnActorLocation;

	UUserWidget* CurrentDefaultUserWidget;

	UUserWidget* CurrentSniperUserWidget;

	AMultiShootGameEnemyCharacter* TargetTakeDownCharacter;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	USceneComponent* GetFPSCameraSceneComponent() const;

	UCameraComponent* GetCameraComponent() const;

	AMultiShootGameFPSCamera* GetCurrentFPSCamera() const;

	bool GetAimed() const;

	EWeaponMode GetWeaponMode() const;

	void SetTakeDown(bool Value);
};
