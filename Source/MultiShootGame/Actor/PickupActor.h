// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerupActor.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/Actor.h"
#include "PickupActor.generated.h"

UCLASS()
class MULTISHOOTGAME_API APickupActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UDecalComponent* DecalComponent;

	UPROPERTY(EditInstanceOnly, Category = "PickupActor")
	TSubclassOf<APowerupActor> PowerupClass;

	void Respawn();

	void RespawnBool();

	APowerupActor* PowerupInstance;

	UPROPERTY(EditInstanceOnly, Category = "PickupActor")
	float CooldownDuration = 30.0f;

	UPROPERTY(EditInstanceOnly, Category = "PickupActor")
	float RespawnBoolDuration = 2.0f;

	FTimerHandle TimerHandle_RespawnTimer;

	FTimerHandle TimerHandle_RespawnBoolTimer;

	bool bRespawn = false;

public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
