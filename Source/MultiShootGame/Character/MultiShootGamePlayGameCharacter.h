// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/Actor.h"
#include "MultiShootGamePlayGameCharacter.generated.h"

UCLASS()
class MULTISHOOTGAME_API AMultiShootGamePlayGameCharacter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMultiShootGamePlayGameCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* CharacterMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneCaptureComponent2D* SceneCaptureComponent2D;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Components")
	USpotLightComponent* SpotLightComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
