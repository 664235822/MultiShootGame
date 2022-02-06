// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MultiShootGameClip.generated.h"

UCLASS()
class MULTISHOOTGAME_API AMultiShootGameClip : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMultiShootGameClip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ClipMeshComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
