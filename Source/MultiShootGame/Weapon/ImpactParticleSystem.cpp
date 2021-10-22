// Fill out your copyright notice in the Description page of Project Settings.


#include "ImpactParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AImpactParticleSystem::AImpactParticleSystem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->OnParticleCollide.AddDynamic(this, &AImpactParticleSystem::OnParticleCollide);
	RootComponent = ParticleSystemComponent;

	InitialLifeSpan = 3.f;
}

void AImpactParticleSystem::OnParticleCollide(FName EventName, float EmitterTime, int32 ParticleTime,
                                              FVector Location, FVector Velocity, FVector Direction,
                                              FVector Normal, FName BoneName, UPhysicalMaterial* PhysMat)
{
	if (DecalMaterial)
	{
		UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DecalMaterial, FVector(80.f, 80.f, 80.f), Location,
		                                       Normal.Rotation());
	}
}
