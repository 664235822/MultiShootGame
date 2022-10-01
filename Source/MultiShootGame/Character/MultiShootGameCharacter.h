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

	virtual void Destroyed() override;

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

public:
	void BeginReload();

protected:
	UFUNCTION(BlueprintCallable)
	void EndReload();

	UFUNCTION(BlueprintCallable)
	void ReloadShowClip(bool Enabled);

	void BeginThrowGrenade();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void EndThrowGrenade_Server();

	void ThrowGrenade();

	UFUNCTION(BlueprintCallable)
	void ThrowGrenadeOut();

	UFUNCTION(Server, Reliable)
	void ThrowGrenadeOut_Server(FRotator Direction, bool MultiThrow);

	UFUNCTION(BlueprintCallable)
	void SpawnGrenade();

	UFUNCTION(Server, Reliable)
	void SpawnGrenade_Server();

	void KnifeAttack();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void BeginKnifeAttack_Server();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void EndKnifeAttack_Server();

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

	UPROPERTY(Replicated, BlueprintReadOnly, Category = Character)
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

	UFUNCTION()
	void OnHealthChanged(UHealthComponent* OwningHealthComponent, float Health, float HealthDelta,
	                     const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable)
	bool CheckStatus(bool CheckAimed, bool CheckThrowGrenade);

	UFUNCTION(BlueprintCallable)
	void EndAction();

	void HandleWalkSpeed();

	UFUNCTION(Server, Reliable)
	void SetWalkSpeed_Server(float Value);

	UFUNCTION(Server, Reliable)
	void SetWeaponMode_Server(EWeaponMode Value);

	UFUNCTION(Server, Reliable)
	void SetBeginThrowGrenade_Server(bool Value);

	UFUNCTION(Server, Reliable)
	void SetThrowingGrenade_Server(bool Value);

	UFUNCTION(Server, Reliable)
	void SetSpawnGrenade_Server(bool Value);

	UFUNCTION(Server, Reliable)
	void SetKnifeAttack_Server(bool Value);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void AttachWeapon_Server();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void PutBackWeapon_Server();

	UFUNCTION(Server, Reliable)
	void PlayAnimMontage_Server(UAnimMontage* AnimMontage, float InPlayRate = 1,
	                            FName StartSectionName = NAME_None);

	UFUNCTION(NetMulticast, Reliable)
	void PlayAnimMontage_Multicast(UAnimMontage* AnimMontage, float InPlayRate = 1,
	                               FName StartSectionName = NAME_None);

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

	UPROPERTY(Replicated)
	bool bBeginThrowGrenade = false;

	UPROPERTY(Replicated)
	bool bThrowingGrenade = false;

	UPROPERTY(Replicated)
	bool bSpawnGrenade = false;

	UPROPERTY(Replicated)
	bool bKnifeAttack = false;

	UPROPERTY(BlueprintReadWrite)
	bool bEnableMovement = true;

	UPROPERTY(BlueprintReadOnly)
	int GrenadeCount;

	UPROPERTY(BlueprintReadOnly)
	AMultiShootGameWeapon* CurrentMainWeapon;

	UPROPERTY(BlueprintReadOnly)
	AMultiShootGameWeapon* CurrentSecondWeapon;

	UPROPERTY(BlueprintReadOnly)
	AMultiShootGameWeapon* CurrentThirdWeapon;

	UPROPERTY(BlueprintReadOnly)
	AMultiShootGameFPSCamera* CurrentFPSCamera;

	UPROPERTY(Replicated, BlueprintReadOnly)
	AMultiShootGameGrenade* CurrentGrenade;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* CurrentGameUserWidget;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* CurrentSniperUserWidget;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintPure, Category = Character)
	FORCEINLINE AMultiShootGameWeapon* GetCurrentMainWeapon() const { return CurrentMainWeapon; }

	UFUNCTION(BlueprintPure, Category = Character)
	FORCEINLINE AMultiShootGameWeapon* GetCurrentSecondWeapon() const { return CurrentSecondWeapon; }

	UFUNCTION(BlueprintPure, Category = Character)
	FORCEINLINE AMultiShootGameWeapon* GetCurrentThirdWeapon() const { return CurrentThirdWeapon; }

	UFUNCTION(BlueprintPure, Category = Character)
	FORCEINLINE AMultiShootGameFPSCamera* GetCurrentFPSCamera() const { return CurrentFPSCamera; }

	UFUNCTION(BlueprintPure, Category = Character)
	FORCEINLINE USceneComponent* GetFPSCameraSceneComponent() const { return FPSCameraSceneComponent; }

	UFUNCTION(BlueprintPure, Category = Character)
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return CameraComponent; }

	UFUNCTION(BlueprintPure, Category = Character)
	FORCEINLINE UHealthComponent* GetHealthComponent() const { return HealthComponent; }

	UFUNCTION(BlueprintPure, Category = Character)
	FORCEINLINE bool GetAimed() const { return bAimed; }

	UFUNCTION(BlueprintPure, Category = Character)
	FORCEINLINE EWeaponMode GetWeaponMode() const { return WeaponMode; }
};
