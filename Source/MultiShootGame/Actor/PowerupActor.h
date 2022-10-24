// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "PowerupActor.generated.h"

/**
 * 
 */
UCLASS()
class MULTISHOOTGAME_API APowerupActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    APowerupActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Components)
    USceneComponent* SceneComponent;

    UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Components)
    UStaticMeshComponent* PowerupMeshComponent;

    UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Components)
    URotatingMovementComponent* RotatingMovementComponent;

    UPROPERTY(EditDefaultsOnly, Category = PowerupActor)
    float PowerupInterval = 0.0f;

    UPROPERTY(EditDefaultsOnly, Category = PowerupActor)
    int32 TotalNumberOfTicks = 0;

    FTimerHandle TimerHandle_PowerupTick;

    int32 TicksProcessed;

    UFUNCTION()
    void OnTickPowerup();
    
    bool bIsPowerupActive = false;

    UFUNCTION(BlueprintImplementableEvent, Category = PowerupActor)
    void OnPowerupStateChanged(bool bNewIsActive);

public:

    void ActivatedPowerUp(AActor* OtherActor);

    UFUNCTION(BlueprintImplementableEvent, Category = PowerupActor)
    void OnActivated(AActor* OtherActor);

    UFUNCTION(BlueprintImplementableEvent, Category = PowerupActor)
    void OnPowerupTicked();

    UFUNCTION(BlueprintImplementableEvent, Category = PowerupActor)
    void OnExpired();
};
