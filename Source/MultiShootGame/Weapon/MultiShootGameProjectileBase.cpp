// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiShootGameProjectileBase.h"

// Sets default values
AMultiShootGameProjectileBase::AMultiShootGameProjectileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void AMultiShootGameProjectileBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMultiShootGameProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMultiShootGameProjectileBase::ProjectileInitialize(float Damage)
{
	BaseDamage = Damage;
}
