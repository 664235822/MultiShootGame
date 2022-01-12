// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiShootGameEnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMultiShootGameEnemyCharacter::AMultiShootGameEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

// Called when the game starts or when spawned
void AMultiShootGameEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMultiShootGameEnemyCharacter::OnHealthChanged(UHealthComponent* OwningHealthComponent, float Health,
	float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	{
		if (Health <= 0.0f && !HealthComponent->bDied)
		{
			Death();
		}
	}
}

void AMultiShootGameEnemyCharacter::Death()
{
	HealthComponent->bDied = true;

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMovementComponent()->SetActive(false);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetAllBodiesPhysicsBlendWeight(0.4f);
	GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
	GetMesh()->GetAnimInstance()->StopAllMontages(0);
}

// Called every frame
void AMultiShootGameEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMultiShootGameEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

