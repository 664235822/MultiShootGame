// Fill out your copyright notice in the Description page of Project Settings.

#include "HitEffectComponent.h"
#include "MultiShootGame/MultiShootGame.h"
#include "MultiShootGame/ParticleSystem/ImpactParticleSystem.h"

// Sets default values for this component's properties
UHitEffectComponent::UHitEffectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHitEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UHitEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHitEffectComponent::PlayHitEffect(EPhysicalSurface SurfaceType, FVector HitPoint, FRotator Rotation)
{
	TSubclassOf<AImpactParticleSystem> SelectEffect = nullptr;

	switch (SurfaceType)
	{
	case SURFACE_CHARACTER:
	case SURFACE_HEAD:
		SelectEffect = FleshImpactEffect;
		break;
	case SURFACE_STONE:
		SelectEffect = StoneImpactEffect;
		break;
	case SURFACE_WOOD:
		SelectEffect = WoodImpactEffect;
		break;
	default:
		SelectEffect = DefaultImpactEffect;
		break;
	}

	GetWorld()->SpawnActor<AActor>(SelectEffect, HitPoint, Rotation);
}
