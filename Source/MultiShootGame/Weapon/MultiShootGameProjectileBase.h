// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MultiShootGameProjectileBase.generated.h"

UCLASS()
class MULTISHOOTGAME_API AMultiShootGameProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMultiShootGameProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ProjectileInitialize(float Damage);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	float BaseDamage = 20.f;
};
