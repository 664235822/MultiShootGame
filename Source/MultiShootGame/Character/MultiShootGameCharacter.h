// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "MatineeCameraShake.h"
#include "MultiShootGameEnemyCharacter.h"
#include "MultiShootGame/Enum//EWeaponMode.h"
#include "MultiShootGame/Component/HealthComponent.h"
#include "MultiShootGame/Component//HitEffectComponent.h"
#include "MultiShootGame/Weapon/MultiShootGameGrenade.h"
#include "MultiShootGame/Weapon/MultiShootGameFPSCamera.h"
#include "MultiShootGame/Weapon/MultiShootGameWeapon.h"
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

	void BeginSecondWeaponReload();

	UFUNCTION(BlueprintCallable)
	void EndReload();

	UFUNCTION(BlueprintCallable)
	void ReloadShowClip(bool Enabled);

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

	void ToggleMainWeapon();

	void ToggleSecondWeapon();

	void ToggleThirdWeapon();

	void ToggleWeaponUp();

	void ToggleWeaponDown();

	UFUNCTION(BlueprintCallable)
	void ToggleWeaponBegin();

	UFUNCTION(BlueprintCallable)
	void ToggleWeaponEnd();

	UFUNCTION(BlueprintCallable)
	void Hit();

	UFUNCTION(BlueprintCallable)
	void FillUpWeaponBullet();
	
	UFUNCTION(BlueprintCallable)
	void Death();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	USceneComponent* FPSCameraSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	USceneComponent* MainWeaponSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	USceneComponent* SecondWeaponSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	USceneComponent* ThirdWeaponSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	USceneComponent* GrenadeSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	USkeletalMeshComponent* KnifeSkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UAudioComponent* DeathAudioComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UHitEffectComponent* HitEffectComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character)
	TSubclassOf<AMultiShootGameWeapon> MainWeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character)
	TSubclassOf<AMultiShootGameWeapon> SecondWeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character)
	TSubclassOf<AMultiShootGameWeapon> ThirdWeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character)
	TSubclassOf<AMultiShootGameGrenade> GrenadeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character)
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditDefaultsOnly, Category = Character)
	FName MainWeaponSocketName = "MainWeaponSocket";

	UPROPERTY(EditDefaultsOnly, Category = Character)
	FName BackMainWeaponSocketName = "BackMainWeaponSocket";

	UPROPERTY(EditDefaultsOnly, Category = Character)
	FName SecondWeaponSocketName = "SecondWeaponSocket";

	UPROPERTY(EditDefaultsOnly, Category = Character)
	FName BackSecondWeaponSocketName = "BackSecondWeaponSocket";

	UPROPERTY(EditDefaultsOnly, Category = Character)
	FName ThirdWeaponSocketName = "ThirdWeaponSocket";

	UPROPERTY(EditDefaultsOnly, Category = Character)
	FName BackThirdWeaponSocketName = "BackThirdWeaponSocket";

	UPROPERTY(EditDefaultsOnly, Category = Character)
	FName GrenadeSocketName = "GrenadeSocket";

	UPROPERTY(EditDefaultsOnly, Category = Character)
	FName KnifeSocketName = "KnifeSocket";

	UPROPERTY(EditDefaultsOnly, Category = Character)
	FName HitSocketName = "HitSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character)
	TSubclassOf<AMultiShootGameFPSCamera> FPSCameraClass;

	UPROPERTY(EditDefaultsOnly, Category = Character)
	TSubclassOf<UMatineeCameraShake> MovementCameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = Character)
	TSubclassOf<UMatineeCameraShake> FPSCameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = Character)
	TSubclassOf<UMatineeCameraShake> KnifeCameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = Character)
	TSubclassOf<UMatineeCameraShake> HitCameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = Character)
	UAnimMontage* WeaponOutAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = Character)
	UAnimMontage* ReloadAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = Character)
	UAnimMontage* SecondWeaponReloadAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = Character)
	UAnimMontage* ThirdWeaponReloadAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = Character)
	UAnimMontage* ThrowGrenadeAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = Character)
	UAnimMontage* KnifeAttackAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character)
	TSubclassOf<UUserWidget> GameUserWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character)
	TSubclassOf<UUserWidget> SniperUserWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = Character)
	EWeaponMode WeaponMode = EWeaponMode::MainWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character)
	float KnifeDamage = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character)
	float GrenadeDamage = 150.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character)
	float CameraPitchClamp = 60.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character)
	float MaxDeadTimeDilationDelay = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character)
	int MaxGrenadeCount = 4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character)
	float DeathRagdollWeight = 0.3;

	UPROPERTY(BlueprintReadOnly)
	AMultiShootGameGrenade* CurrentGrenade;

	UFUNCTION()
	void OnHealthChanged(UHealthComponent* OwningHealthComponent, float Health, float HealthDelta,
	                     const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void ToggleUseControlRotation(bool Enabled);

	UFUNCTION(BlueprintCallable)
	bool CheckStatus(bool CheckAimed, bool CheckThrowGrenade);

	UFUNCTION(BlueprintCallable)
	void EndAction();

	void HandleWalkSpeed();

	UFUNCTION(BlueprintCallable)
	void AttachWeapon(bool MainWeapon, bool SecondWeapon, bool ThirdWeapon);

	bool bFired = false;

	bool bFastRun = false;

	bool bJump = false;

	UPROPERTY(BlueprintReadOnly)
	bool bMoving = false;

	UPROPERTY(BlueprintReadWrite)
	bool bDetectingClimb = false;

	UPROPERTY(BlueprintReadOnly)
	bool bAimed = false;

	bool bReloading = false;

	bool bSecondWeaponReloading = false;

	bool bToggleWeapon = false;

	bool bBeginThrowGrenade = false;

	bool bThrowingGrenade = false;

	bool bSpawnGrenade = false;

	bool bKnifeAttack = false;

	bool bUseControlRotation = false;

	UPROPERTY(BlueprintReadWrite)
	bool bEnableMovement = true;

	UPROPERTY(BlueprintReadOnly)
	int GrenadeCount;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* CurrentGameUserWidget;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* CurrentSniperUserWidget;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void BeginReload();

	USceneComponent* GetFPSCameraSceneComponent() const;

	UCameraComponent* GetCameraComponent() const;

	UPROPERTY(BlueprintReadOnly)
	AMultiShootGameWeapon* CurrentMainWeapon;

	UPROPERTY(BlueprintReadOnly)
	AMultiShootGameWeapon* CurrentSecondWeapon;

	UPROPERTY(BlueprintReadOnly)
	AMultiShootGameWeapon* CurrentThirdWeapon;

	UPROPERTY(BlueprintReadOnly)
	AMultiShootGameFPSCamera* CurrentFPSCamera;

	bool GetAimed() const;

	EWeaponMode GetWeaponMode() const;

	void SetEnableMovement(bool Value);

	UHealthComponent* GetHealthComponent() const;
};
