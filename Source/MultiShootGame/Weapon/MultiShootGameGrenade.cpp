// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiShootGameGrenade.h"
#include "Core/Private/Windows/WindowsRunnableThread.h"
#include "Kismet/GameplayStatics.h"
#include "MultiShootGame/Character/MultiShootGameCharacter.h"

// Sets default values
AMultiShootGameGrenade::AMultiShootGameGrenade()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	GrenadeComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrenadeComponnet"));
	RootComponent = GrenadeComponent;

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetupAttachment(GrenadeComponent);
	ParticleSystemComponent->bAutoActivate = false;
	ParticleSystemComponent->SetIsReplicated(true);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(
		TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bAutoActivate = false;
	ProjectileMovementComponent->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void AMultiShootGameGrenade::BeginPlay()
{
	Super::BeginPlay();
}

void AMultiShootGameGrenade::ThrowGrenade_Multicast_Implementation()
{
	ParticleSystemComponent->Activate();
}

void AMultiShootGameGrenade::Explode_Multicast_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticleSystem, GetActorLocation());
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ExplosionSoundCue, GetActorLocation());
}

void AMultiShootGameGrenade::Explode()
{
	Explode_Multicast();
	
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), GrenadeCameraShakeClass, GetActorLocation(), 0, DamageRadius);

	const TArray<AActor*> IgnoreActors;

	//UGameplayStatics::ApplyRadialDamage(GetWorld(), BaseDamage, GetActorLocation(), DamageRadius, DamageTypeClass,
	//									IgnoreActors, GetOwner(), GetOwner()->GetInstigatorController());

	Destroy();
}

// Called every frame
void AMultiShootGameGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMultiShootGameGrenade::ThrowGrenade_Server_Implementation(FRotator Direction, bool MultiThrow)
{
	Direction += ThrowRotatorPlus;

	ProjectileMovementComponent->Velocity = Direction.Vector() * ProjectileMovementComponent->InitialSpeed;
	if (MultiThrow)
	{
		ProjectileMovementComponent->Velocity *= 1.5f;
	}
	ProjectileMovementComponent->Activate();

	ThrowGrenade_Multicast();

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMultiShootGameGrenade::Explode, ExplodedDelay);
}

void AMultiShootGameGrenade::ProjectileInitialize(float Damage)
{
	Super::ProjectileInitialize(Damage);

	ThrowGrenade_Server(GetActorRotation(), false);
}
