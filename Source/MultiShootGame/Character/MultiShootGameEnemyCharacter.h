// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "MultiShootGame/Component/HealthComponent.h"
#include "MultiShootGame/GameMode/MultiShootGameGameInstance.h"
#include "MultiShootGame/GameMode/MultiShootGameGameModeBase.h"
#include "MultiShootGame/Weapon/MultiShootGameEnemyWeapon.h"
#include "Perception/AIPerceptionComponent.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UAudioComponent* DeathAudioComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Enemy)
	TSubclassOf<AMultiShootGameEnemyWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Enemy)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Enemy)
	float DeathDestroyDelay = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = Enemy)
	FName WeaponSocketName = "WeaponSocket";

	UPROPERTY(BlueprintReadOnly)
	AMultiShootGameEnemyWeapon* CurrentWeapon;

	UPROPERTY(BlueprintReadOnly)
	AMultiShootGameGameModeBase* CurrentGameMode;

	UPROPERTY(BlueprintReadOnly)
	UMultiShootGameGameInstance* CurrentGameInstance;

	FTimerHandle TimerHandle;

	UFUNCTION()
	void OnHealthChanged(UHealthComponent* OwningHealthComponent, float Health, float HealthDelta,
	                     const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void OnHeadShot(AActor* DamageCauser);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

	UFUNCTION(BlueprintCallable, Category = Enemy)
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = Enemy)
	void StopFire();
};
