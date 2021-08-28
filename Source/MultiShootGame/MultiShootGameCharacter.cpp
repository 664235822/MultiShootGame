// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiShootGameCharacter.h"
#include "MultiShootGameProjectile.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/InputSettings.h"

AMultiShootGameCharacter::AMultiShootGameCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SprintArmComponent"));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void AMultiShootGameCharacter::BeginPlay()
{
	Super::BeginPlay();
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
	PlayAnimMontage(AimAnimMontage);
}

void AMultiShootGameCharacter::EndAim()
{
	StopAnimMontage(AimAnimMontage);
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
	PlayerInputComponent->BindAction("Aim",IE_Pressed,this,&AMultiShootGameCharacter::BeginAim);
	PlayerInputComponent->BindAction("Aim",IE_Released,this,&AMultiShootGameCharacter::EndAim);
	
}
