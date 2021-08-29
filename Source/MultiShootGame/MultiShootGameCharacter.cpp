// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiShootGameCharacter.h"
#include "AIController.h"
#include "MultiShootGameProjectile.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AMultiShootGameCharacter::AMultiShootGameCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bFindCameraComponentWhenViewTarget = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SprintArmComponent"));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	FPSCameraSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("FPSCameraSceneComponent"));
	FPSCameraSceneComponent->SetupAttachment(RootComponent);
}

void AMultiShootGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CurrentWeapon = GetWorld()->SpawnActor<AMultiShootGameWeapon>(WeaponClass, FVector::ZeroVector,
	                                                              FRotator::ZeroRotator,
	                                                              SpawnParameters);
	CurrentFPSCamera = GetWorld()->SpawnActor<AMultiShootGameFPSCamera>(FPSCameraClass, FVector::ZeroVector,
	                                                                    FRotator::ZeroRotator,
	                                                                    SpawnParameters);

	if (CurrentWeapon)
	{
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
		                                 WeaponSocketName);
	}

	if (CurrentFPSCamera)
	{
		CurrentFPSCamera->SetOwner(this);
		CurrentFPSCamera->AttachToComponent(FPSCameraSceneComponent,
		                                    FAttachmentTransformRules::SnapToTargetIncludingScale);

		CurrentFPSCamera->SetActorHiddenInGame(true);
	}
}

void AMultiShootGameCharacter::OnFire()
{
}

void AMultiShootGameCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void AMultiShootGameCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void AMultiShootGameCharacter::BeginFastRun()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void AMultiShootGameCharacter::EndFastRun()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void AMultiShootGameCharacter::BeginCrouch()
{
	Crouch();
}

void AMultiShootGameCharacter::EndCrouch()
{
	UnCrouch();
}

void AMultiShootGameCharacter::BeginZoom()
{
	bWantToZoom = true;
}

void AMultiShootGameCharacter::EndZoom()
{
	bWantToZoom = false;
}

void AMultiShootGameCharacter::BeginAim()
{
	bAimed = true;

	PlayAnimMontage(AimAnimMontage);

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->SetViewTargetWithBlend(CurrentFPSCamera, 0.1f);

	CurrentFPSCamera->SetActorHiddenInGame(false);
	CurrentWeapon->SetActorHiddenInGame(true);
	GetMesh()->SetHiddenInGame(true);
}

void AMultiShootGameCharacter::EndAim()
{
	bAimed = false;

	StopAnimMontage(AimAnimMontage);

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->SetViewTargetWithBlend(this, 0.1f);

	CurrentFPSCamera->SetActorHiddenInGame(true);
	CurrentWeapon->SetActorHiddenInGame(false);
	GetMesh()->SetHiddenInGame(false);
}

void AMultiShootGameCharacter::AimLookAround()
{
	const FVector StartLocation = FPSCameraSceneComponent->GetComponentLocation();

	const FVector CameraLocation = CameraComponent->GetComponentLocation();
	const FRotator CameraRotation = CameraComponent->GetComponentRotation();
	const FVector TargetLocation = CameraLocation + CameraRotation.Vector() * 5000.f;

	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);

	const FRotator TargetRotation = FRotator(0, LookAtRotation.Yaw - 90.f, LookAtRotation.Pitch * -1.f);

	FPSCameraSceneComponent->SetWorldRotation(TargetRotation);
}

void AMultiShootGameCharacter::Jump()
{
	Super::Jump();

	PlayAnimMontage(JumpAnimMontage);
}

void AMultiShootGameCharacter::StopJumping()
{
	Super::StopJumping();

	PlayAnimMontage(JumpAnimMontage, 1, FName("down"));
}

void AMultiShootGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bAimed)
	{
		AimLookAround();
	}
}

void AMultiShootGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMultiShootGameCharacter::OnFire);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AMultiShootGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMultiShootGameCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	// Bind fastrun events
	PlayerInputComponent->BindAction("FastRun", IE_Pressed, this, &AMultiShootGameCharacter::BeginFastRun);
	PlayerInputComponent->BindAction("FastRun", IE_Released, this, &AMultiShootGameCharacter::EndFastRun);

	// Bind aim events
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AMultiShootGameCharacter::BeginAim);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AMultiShootGameCharacter::EndAim);
}
