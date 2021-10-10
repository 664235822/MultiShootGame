// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ImpactParticleSystem.generated.h"

UCLASS()
class MULTISHOOTGAME_API AImpactParticleSystem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AImpactParticleSystem();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* ParticleSystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Impact")
	UMaterial* DecalMaterial;

	UFUNCTION()
	void OnParticleCollide(FName EventName, float EmitterTime, int32 ParticleTime, FVector Location, FVector Velocity,
	                       FVector Direction, FVector Normal, FName BoneName, UPhysicalMaterial* PhysMat);
};
