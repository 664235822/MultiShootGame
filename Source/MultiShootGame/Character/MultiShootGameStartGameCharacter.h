// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SpotLightComponent.h"
#include "MultiShootGameStartGameCharacter.generated.h"

UCLASS()
class MULTISHOOTGAME_API AMultiShootGameStartGameCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMultiShootGameStartGameCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
    USkeletalMeshComponent* CharacterMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	USkeletalMeshComponent* WeaponMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	USceneCaptureComponent2D* SceneCaptureComponent2D;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Components)
	USpotLightComponent* SpotLightComponent;

	UPROPERTY(EditDefaultsOnly, Category = Character)
	FName WeaponSocketName = "StartWeaponSocket";

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
