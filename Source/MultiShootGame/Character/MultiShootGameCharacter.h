// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
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

	UFUNCTION(BlueprintCallable)
	void StartFire();

	UFUNCTION(BlueprintCallable)
	void StopFire();

	void MoveForward(float Value);

	void MoveRight(float Value);

	UFUNCTION(BlueprintCallable)
	void BeginFastRun();

	UFUNCTION(BlueprintCallable)
	void EndFastRun();

	UFUNCTION(BlueprintCallable)
	void BeginCrouch();

	UFUNCTION(BlueprintCallable)
	void EndCrouch();

	void ToggleCrouch();

	UFUNCTION(BlueprintCallable)
	void BeginAim();

	UFUNCTION(BlueprintCallable)
	void EndAim();

public:
	UFUNCTION(Server, Unreliable)
	void Fire_Server(FWeaponInfo WeaponInfo, FVector MuzzleLocation, FRotator ShotTargetDirection,
	                 FName MuzzleSocketName);

	UFUNCTION(NetMulticast, Unreliable)
	void Fire_Multicast(FWeaponInfo WeaponInfo, FName MuzzleSocketName);

	UFUNCTION(Server, Unreliable)
	void ThrowBulletShell_Server(TSubclassOf<AMultiShootGameBulletShell> BulletShellClass, FVector BulletShellLocation,
	                             FRotator BulletShellRotation);

	UFUNCTION(BlueprintCallable)
	void BeginReload();

protected:
	void BeginSecondWeaponReload();

	UFUNCTION(BlueprintCallable)
	void EndReload();

	UFUNCTION(BlueprintCallable)
	void ReloadShowClip(bool Enabled);

	UFUNCTION(BlueprintCallable)
	void BeginThrowGrenade();

	UFUNCTION(BlueprintCallable)
	void EndThrowGrenade();

	UFUNCTION(BlueprintCallable)
	void ThrowGrenade();

	UFUNCTION(BlueprintCallable)
	void ThrowGrenadeOut();

	UFUNCTION(Server, Reliable)
	void ThrowGrenadeOut_Server(FRotator Direction, bool MultiThrow);

	UFUNCTION(BlueprintCallable)
	void SpawnGrenade();

	UFUNCTION(Server, Reliable)
	void SpawnGrenade_Server();

	UFUNCTION(BlueprintCallable)
	void KnifeAttack();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void BeginKnifeAttack_Server();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void EndKnifeAttack_Server();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void KnifeHit_Server();

	UFUNCTION(BlueprintCallable)
	void ToggleMainWeapon();

	UFUNCTION(BlueprintCallable)
	void ToggleSecondWeapon();

	UFUNCTION(BlueprintCallable)
	void ToggleThirdWeapon();

	void ToggleWeaponUp();

	void ToggleWeaponDown();

	UFUNCTION(BlueprintCallable)
	void ToggleWeaponBegin();

	UFUNCTION(BlueprintCallable)
	void ToggleWeaponEnd();

	UFUNCTION(BlueprintCallable)
	void FillUpWeaponBullet();

	UFUNCTION(BlueprintCallable)
	void ToggleView();

	void ToggleFirstPersonView();

	void ToggleThirdPersonView();

	UFUNCTION(BlueprintCallable)
	void Inspect();

	UFUNCTION()
	void HeadShot(AActor* DamageCauser);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Death_Server();

	UFUNCTION(NetMulticast, Reliable)
	void Death_Multicast();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Reborn_Server();

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character)
	TSubclassOf<AActor> PlayerStartClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character)
	TSubclassOf<AActor> CharacterClass;

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
	TSubclassOf<UCameraShakeBase> MovementCameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = Character)
	TSubclassOf<UCameraShakeBase> KnifeCameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = Character)
	TSubclassOf<UCameraShakeBase> HitCameraShakeClass;

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
	TSubclassOf<UUserWidget> ServerGameUserWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character)
	TSubclassOf<UUserWidget> MobileJoystickUserWidgetClass;

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

	UPROPERTY(EditDefaultsOnly, Category = Character)
	bool bShowMobileJoystick = false;

	UFUNCTION()
	void OnHealthChanged(UHealthComponent* OwningHealthComponent, float Health, float HealthDelta,
	                     const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable)
	bool CheckStatus(bool CheckAimed, bool CheckThrowGrenade);

	UFUNCTION(BlueprintCallable)
	void EndAction(bool CheckToggleView);

	void HandleWalkSpeed(bool FastRun);

	UFUNCTION(Server, Reliable)
	void SetWalkSpeed_Server(float Value);

	UFUNCTION(Server, Reliable)
	void SetWeaponMode_Server(EWeaponMode Value);

	UFUNCTION(Server, Reliable)
	void SetFastRun_Server(bool Value);

	UFUNCTION(Server, Reliable)
	void SetAimed_Server(bool Value);

	UFUNCTION(Server, Reliable)
	void SetBeginThrowGrenade_Server(bool Value);

	UFUNCTION(Server, Reliable)
	void SetThrowingGrenade_Server(bool Value);

	UFUNCTION(Server, Reliable)
	void SetSpawnGrenade_Server(bool Value);

	UFUNCTION(Server, Reliable)
	void SetKnifeAttack_Server(bool Value);

	UFUNCTION(Server, Reliable)
	void SetGrenadeCount_Server(int Value);

	UFUNCTION(Server, Reliable)
	void SetToggleView_Server(bool Value);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void AttachWeapon_Server();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void PutBackWeapon_Server();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void PlayAnimMontage_Server(UAnimMontage* AnimMontage, float InPlayRate = 1,
	                            FName StartSectionName = NAME_None);

	UFUNCTION(NetMulticast, Reliable)
	void PlayAnimMontage_Multicast(UAnimMontage* AnimMontage, float InPlayRate = 1,
	                               FName StartSectionName = NAME_None);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void StopAnimMontage_Server(UAnimMontage* AnimMontage);

	UFUNCTION(NetMulticast, Reliable)
	void StopAnimMontage_Multicast(UAnimMontage* AnimMontage);

	UFUNCTION(Server, Reliable)
	void HandleWeaponMesh_Server();

	UFUNCTION(NetMulticast, Reliable)
	void HandleWeaponMesh_Multicast();

	void CheckShowSight(float DeltaSeconds);

	void CheckWeaponInitialized();

	float CurrentShowSight = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = GameMode)
	float ShowSightDelay = 1.f;

	UPROPERTY(Replicated, BlueprintReadOnly)
	float Pitch = 0.0f;

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bShowSight = false;

	bool bFired = false;

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bFastRun = false;

	UPROPERTY(BlueprintReadOnly)
	bool bMoving = false;

	UPROPERTY(Replicated, BlueprintReadWrite)
	bool bDetectingClimb = false;

	UPROPERTY(Replicated, BlueprintReadOnly)
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

	UPROPERTY(Replicated)
	bool bToggleView = false;

	UPROPERTY(BlueprintReadWrite)
	bool bEnableMovement = true;

	UPROPERTY(Replicated, BlueprintReadOnly)
	int GrenadeCount;

	FTimerHandle TimerHandle;

	UPROPERTY(Replicated, BlueprintReadOnly)
	AMultiShootGameWeapon* CurrentMainWeapon;

	UPROPERTY(Replicated, BlueprintReadOnly)
	AMultiShootGameWeapon* CurrentSecondWeapon;

	UPROPERTY(Replicated, BlueprintReadOnly)
	AMultiShootGameWeapon* CurrentThirdWeapon;

	UPROPERTY(BlueprintReadOnly)
	AMultiShootGameFPSCamera* CurrentFPSCamera;

	UPROPERTY(BlueprintReadOnly)
	AMultiShootGameGrenade* CurrentGrenade;

	UPROPERTY(BlueprintReadOnly)
	AGameModeBase* CurrentGameMode;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* CurrentGameUserWidget;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* CurrentMobileJoystickUserWidget;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void OnEnemyKilled();

	void OnHeadshot();

	void OnDeath();

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
	FORCEINLINE bool GetFired() const { return bFired; }
	
	UFUNCTION(BlueprintPure, Category = Character)
	FORCEINLINE bool GetAimed() const { return bAimed; }

	UFUNCTION(BlueprintPure, Category = Character)
	FORCEINLINE bool GetReloading() const { return bReloading; }

	UFUNCTION(BlueprintPure, Category = Character)
	FORCEINLINE bool GetToggleWeapon() const { return bToggleWeapon; }

	UFUNCTION(BlueprintPure, Category = Character)
	FORCEINLINE bool GetToggleViewed() const { return bToggleView; }

	UFUNCTION(BlueprintPure, Category = Character)
	FORCEINLINE EWeaponMode GetWeaponMode() const { return WeaponMode; }
};
