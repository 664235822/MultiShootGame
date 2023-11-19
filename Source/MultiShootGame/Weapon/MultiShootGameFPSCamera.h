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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	USkeletalMeshComponent* ArmsMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<UUserWidget> SniperUserWidgetClass;

	UPROPERTY(EditAnywhere, Category = Weapon)
	FName WeaponSocketName = FName("Weapon");

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	float ZoomedFOV = 40.0f;

	float DefaultFOV;

	UPROPERTY(EditDefaultsOnly, Category = Weapon, meta = (ClampMin = 0.1f, ClampMax = 100.0f))
	float ZoomInterpSpeed = 20.0f;

	UPROPERTY(BlueprintReadOnly)
	float Speed;

	UPROPERTY(BlueprintReadOnly)
	bool bFired;

	UPROPERTY(BlueprintReadOnly)
	bool bAimed;

	UPROPERTY(BlueprintReadOnly)
	bool bReloading;

	UPROPERTY(BlueprintReadOnly)
	bool bToggleWeapon;

	UPROPERTY(BlueprintReadOnly)
	bool bInspect;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* CurrentSniperUserWidget;

	UPROPERTY(BlueprintReadOnly)
	AMultiShootGameMagazineClip* CurrentMagazineClip;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Fire() override;

	virtual bool BulletCheck(AMultiShootGameCharacter* MyOwner) override;

	virtual void BulletFire(AMultiShootGameCharacter* MyOwner) override;

	UFUNCTION(BlueprintCallable)
	virtual void ReloadShowMagazineClip(bool Enabled) override;

	void InspectBegin();

	UFUNCTION(BlueprintCallable)
	void InspectEnd();

	UFUNCTION(BlueprintCallable)
	void SetWeaponMesh();

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetWeaponInfo(FWeaponInfo Info);

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void BeginAim(EWeaponMode WeaponMode);

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void EndAim();

	UFUNCTION(BlueprintPure, Category = Weapon)
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return CameraComponent; }
};
