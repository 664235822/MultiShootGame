// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MultiShootGameMagazineClip.generated.h"

UCLASS()
class MULTISHOOTGAME_API AMultiShootGameMagazineClip : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMultiShootGameMagazineClip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	UStaticMeshComponent* MagazineClipMeshComponent;

	FTimerHandle TimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	float DestroyDelay = 2.0f;

	void DestroyMagazineClip();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ThrowMagazineClip(UStaticMesh* MagazineClipMesh);
};
