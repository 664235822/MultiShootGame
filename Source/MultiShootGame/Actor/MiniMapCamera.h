// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MiniMapCamera.generated.h"

UCLASS()
class MULTISHOOTGAME_API AMiniMapCamera : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMiniMapCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	USceneCaptureComponent2D* MinimapCameraComponent2D;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="MinimapCamera")
	float CameraHeight = 1000.f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
