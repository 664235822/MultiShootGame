// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupActor.h"

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

    SetReplicates(true);
}

// Called when the game starts or when spawned
void APickupActor::BeginPlay()
{
    Super::BeginPlay();

    if (GetLocalRole() == ROLE_Authority)
    {
        Respawn();
    }
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
}

void APickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    if (GetLocalRole() == ROLE_Authority && PowerupInstance)
    {
        PowerupInstance->ActivatedPowerUp(OtherActor);

        PowerupInstance = nullptr;

        GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &APickupActor::Respawn, CooldownDuration);
    }
}
