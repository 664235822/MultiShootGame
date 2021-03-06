// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupActor.h"

#include "Kismet/GameplayStatics.h"
#include "MultiShootGame/Character/MultiShootGameCharacter.h"

// Sets default values
APickupActor::APickupActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetSphereRadius(75.0f);
	RootComponent = SphereComponent;

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetRelativeRotation(FRotator(90.0f, 0, 0));
	DecalComponent->DecalSize = FVector(64.0f, 75.0f, 75.0f);
	DecalComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APickupActor::BeginPlay()
{
	Super::BeginPlay();

	Respawn();
}

void APickupActor::Respawn()
{
	if (PowerupClass == nullptr)
	{
		return;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	PowerupInstance = GetWorld()->SpawnActor<APowerupActor>(PowerupClass, GetTransform(), SpawnParameters);

	GetWorldTimerManager().SetTimer(TimerHandle_RespawnBoolTimer, this, &APickupActor::RespawnBool,
	                                RespawnBoolDuration);
}

void APickupActor::RespawnBool()
{
	bRespawn = true;
}

void APickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (Cast<AMultiShootGameCharacter>(OtherActor) && bRespawn)
	{
		PowerupInstance->ActivatedPowerUp(OtherActor);

		PowerupInstance = nullptr;

		bRespawn = false;

		GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &APickupActor::Respawn, CooldownDuration);
	}
}
