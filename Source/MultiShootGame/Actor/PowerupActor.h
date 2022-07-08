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

    UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Component")
    USceneComponent* SceneComponent;

    UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Component")
    UStaticMeshComponent* PowerupMeshComponent;

    UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Component")
    UPointLightComponent* PointLightComponent;

    UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Component")
    URotatingMovementComponent* RotatingMovementComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Powerup")
    float PowerupInterval = 0.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Powerup")
    int32 TotalNumberOfTicks = 0;

    FTimerHandle TimerHandle_PowerupTick;

    int32 TicksProcessed;

    UFUNCTION()
    void OnTickPowerup();

    UPROPERTY(ReplicatedUsing = OnRep_PowerupActive)
    bool bIsPowerupActive = false;

    UFUNCTION()
    void OnRep_PowerupActive();

    UFUNCTION(BlueprintImplementableEvent, Category = "Powerup")
    void OnPowerupStateChanged(bool bNewIsActive);

public:

    void ActivatedPowerUp(AActor* OtherActor);

    UFUNCTION(BlueprintImplementableEvent, Category = "Powerup")
    void OnActivated(AActor* OtherActor);

    UFUNCTION(BlueprintImplementableEvent, Category = "Powerup")
    void OnPowerupTicked();

    UFUNCTION(BlueprintImplementableEvent, Category = "Powerup")
    void OnExpired();
};
