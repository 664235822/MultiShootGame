// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerupActor.h"

APowerupActor::APowerupActor()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	PowerupMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PowerupMeshComponent"));
	PowerupMeshComponent->SetupAttachment(SceneComponent);

	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLightComponent"));
	PointLightComponent->SetupAttachment(PowerupMeshComponent);

	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovementComponent"));
}

void APowerupActor::BeginPlay()
{
	Super::BeginPlay();
}

void APowerupActor::OnTickPowerup()
{
	TicksProcessed++;

	OnPowerupTicked();

	if (TicksProcessed >= TotalNumberOfTicks)
	{
		OnExpired();

		bIsPowerupActive = false;

		GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
	}
}

void APowerupActor::ActivatedPowerUp(AActor* OtherActor)
{
	OnActivated(OtherActor);

	bIsPowerupActive = true;

	if (PowerupInterval > 0.0f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick, this, &APowerupActor::OnTickPowerup,
		                                PowerupInterval, true);
	}
	else
	{
		OnTickPowerup();
	}
}
