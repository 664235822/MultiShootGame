// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MultiShootGame/ParticleSystem/ImpactParticleSystem.h"
#include "HitEffectComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTISHOOTGAME_API UHitEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHitEffectComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = HitEffect)
	TSubclassOf<AImpactParticleSystem> DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = HitEffect)
	TSubclassOf<AImpactParticleSystem> FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = HitEffect)
	TSubclassOf<AImpactParticleSystem> StoneImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = HitEffect)
	TSubclassOf<AImpactParticleSystem> WoodImpactEffect;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PlayHitEffect(EPhysicalSurface SurfaceType, FVector HitPoint, FRotator Rotation);
};
