// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiShootGameGrenade.h"

#include "Core/Private/Windows/WindowsRunnableThread.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMultiShootGameGrenade::AMultiShootGameGrenade()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

	GrenadeComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrenadeComponnet"));
	RootComponent = GrenadeComponent;

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetupAttachment(GrenadeComponent);
	ParticleSystemComponent->bAutoActivate = false;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(
		TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AMultiShootGameGrenade::BeginPlay()
{
	Super::BeginPlay();
}

void AMultiShootGameGrenade::Explode()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticleSystem, GetActorLocation());
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ExplosionSoundCue, GetActorLocation());

	Destroy();
}

// Called every frame
void AMultiShootGameGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMultiShootGameGrenade::ThrowGrenade(FRotator Direction)
{
	ProjectileMovementComponent->Velocity = Direction.Vector() * ProjectileMovementComponent->InitialSpeed;
	ProjectileMovementComponent->Activate();

	ParticleSystemComponent->Activate();

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMultiShootGameGrenade::Explode, ExplodedDelay);
}
