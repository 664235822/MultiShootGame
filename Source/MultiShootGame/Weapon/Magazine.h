// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Magazine.generated.h"

UCLASS()
class MULTISHOOTGAME_API AMagazine : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMagazine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Componennt")
	UStaticMeshComponent* MagazineMeshComponent;

	FTimerHandle TimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Magazine")
	float DestroyDelay = 2.0f;

	void DestroyMagazine();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ThrowMagazine(UStaticMesh* MagazineMesh);
};
