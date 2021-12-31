// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BulletShell.generated.h"

UCLASS()
class MULTISHOOTGAME_API ABulletShell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletShell();

protected:

	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	UStaticMeshComponent* BulletShellComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovementComponent;

	FTimerHandle TimerHandle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Bullet Shell")
	float DestroyDelay = 4.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Bullet Shell")
	float MinInitialSpeed = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Bullet Shell")
	float MaxInitialSpeed = 300.0f;
	
	void DestroyBulletShell();
	
public:	

	void ThrowBulletShell();

};
