// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiShootGameEnemyCharacter.h"
#include "AIController.h"
#include "MultiShootGameCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMultiShootGameEnemyCharacter::AMultiShootGameEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SprintArmComponent"));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
}

// Called when the game starts or when spawned
void AMultiShootGameEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnHealthChanged.AddDynamic(this, &AMultiShootGameEnemyCharacter::OnHealthChanged);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CurrentWeapon = GetWorld()->SpawnActor<AMultiShootGameEnemyWeapon>(
		WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameters);
	if (CurrentWeapon)
	{
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
		                                 WeaponSocketName);
	}
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

	PlayerInputComponent->BindAxis("MoveForward", this, &AMultiShootGameEnemyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMultiShootGameEnemyCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AMultiShootGameEnemyCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AMultiShootGameEnemyCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMultiShootGameEnemyCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMultiShootGameEnemyCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMultiShootGameEnemyCharacter::Jump);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMultiShootGameEnemyCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMultiShootGameEnemyCharacter::StopFire);
}

FVector AMultiShootGameEnemyCharacter::GetPawnViewLocation() const
{
	if (CameraComponent)
	{
		return CameraComponent->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

void AMultiShootGameEnemyCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void AMultiShootGameEnemyCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void AMultiShootGameEnemyCharacter::StartFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFire();
	}
}

void AMultiShootGameEnemyCharacter::StopFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}

void AMultiShootGameEnemyCharacter::BeginCrouch()
{
	Crouch();
}

void AMultiShootGameEnemyCharacter::EndCrouch()
{
	UnCrouch();
}

void AMultiShootGameEnemyCharacter::DeathDestroy()
{
	Destroy();
}

void AMultiShootGameEnemyCharacter::OnHealthChanged(UHealthComponent* OwningHealthComponent, float Health,
                                                    float HealthDelta,
                                                    const class UDamageType* DamageType,
                                                    class AController* InstigatedBy,
                                                    AActor* DamageCauser)
{
	if (Health <= 0.0f && !HealthComponent->bDied)
	{
		HealthComponent->bDied = true;

		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
		DetachFromControllerPendingDestroy();
		PlayAnimMontage(DeathMontage);
		CurrentWeapon->EnablePhysicsSimulate();

		GetWorldTimerManager().SetTimer(TimerHandle, this, &AMultiShootGameEnemyCharacter::DeathDestroy,
		                                DeathDestroyDelay);
	}
}
