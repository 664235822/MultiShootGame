// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiShootGameRocket.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AMultiShootGameRocket::AMultiShootGameRocket()
{
	RocketComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RocketComponnet"));
	RocketComponent->OnComponentHit.AddDynamic(this, &AMultiShootGameRocket::OnHit);
	RootComponent = RocketComponent;

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleSystemComponent->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileMovementComponent->UpdatedComponent = RocketComponent;
	ProjectileMovementComponent->InitialSpeed = 3000.f;
	ProjectileMovementComponent->MaxSpeed = 3000.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0;
}


void AMultiShootGameRocket::Explode()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticleSystem, GetActorLocation());
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ExplosionSoundCue, GetActorLocation());
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), RocketCameraShakeClass, GetActorLocation(), 0, DamageRadius);

	const TArray<AActor*> IgnoreActors;

	UGameplayStatics::ApplyRadialDamage(GetWorld(), BaseDamage, GetActorLocation(), DamageRadius, DamageTypeClass,
	                                    IgnoreActors, GetOwner(), GetOwner()->GetInstigatorController());

	Destroy();
}

void AMultiShootGameRocket::ProjectileInitialize(float Damage)
{
	Super::ProjectileInitialize(Damage);

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMultiShootGameRocket::Explode, ExplodedDelay);
}

void AMultiShootGameRocket::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                  FVector NormalImpulse,
                                  const FHitResult& Hit)
{
	Explode();
}
