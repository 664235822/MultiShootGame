// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "MultiShootGame/Component/HealthComponent.h"
#include "MultiShootGame/Weapon/MultiShootGameEnemyWeapon.h"
#include "MultiShootGameEnemyCharacter.generated.h"

UCLASS()
class MULTISHOOTGAME_API AMultiShootGameEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMultiShootGameEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void BeginCrouch();

	void EndCrouch();

	void DeathDestroy();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy")
	TSubclassOf<AMultiShootGameEnemyWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Enemy")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy")
	float DeathDestroyDelay = 20.f;

	UPROPERTY(VisibleDefaultsOnly, Category = "Enemy")
	FName WeaponSocketName = "WeaponSocket";

	UPROPERTY(BlueprintReadOnly, Category = "Enemy")
	bool bDied = false;

	AMultiShootGameEnemyWeapon* CurrentWeapon;

	FTimerHandle TimerHandle;

	UFUNCTION()
	void OnHealthChanged(UHealthComponent* OwningHealthComponent, float Health, float HealthDelta,
	                     const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

	UFUNCTION(BlueprintCallable, Category = "Player")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Player")
	void StopFire();
};
