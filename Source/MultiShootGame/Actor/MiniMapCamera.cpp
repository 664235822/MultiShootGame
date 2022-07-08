// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMapCamera.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMiniMapCamera::AMiniMapCamera()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	MinimapCameraComponent2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MinimapCameraComponent2D"));
	MinimapCameraComponent2D->SetupAttachment(SceneComponent);
}

// Called when the game starts or when spawned
void AMiniMapCamera::BeginPlay()
{
	Super::BeginPlay();

	MinimapCameraComponent2D->HiddenActors.Add(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

// Called every frame
void AMiniMapCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Position = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
	Position += FVector(0, 0, CameraHeight);

	FRotator Rotation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetControlRotation();
	Rotation = FRotator(0, Rotation.Yaw, 0);

	SetActorLocation(Position);
	SetActorRotation(Rotation);
}
