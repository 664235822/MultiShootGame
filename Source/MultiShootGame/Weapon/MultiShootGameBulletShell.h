// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MultiShootGameBulletShell.generated.h"

UCLASS()
class MULTISHOOTGAME_API AMultiShootGameBulletShell : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMultiShootGameBulletShell();

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	UStaticMeshComponent* BulletShellComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovementComponent;

	FTimerHandle TimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	float DestroyDelay = 2.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	float MinInitialSpeed = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	float MaxInitialSpeed = 300.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	FRotator ThrowDirection = FRotator(60.f, 180.f, 0);

	void DestroyBulletShell();

public:
	void ThrowBulletShell();
};
