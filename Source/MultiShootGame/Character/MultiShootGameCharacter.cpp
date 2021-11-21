// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiShootGameCharacter.h"
#include "AIController.h"
#include "../Weapon/MultiShootGameProjectile.h"
#include "AnimGraphRuntime/Public/KismetAnimationLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
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

	WeaponSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponSceneComponent"));
	WeaponSceneComponent->SetupAttachment(GetMesh(), WeaponSocketName);

	SniperSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SniperSceneComponent"));
	SniperSceneComponent->SetupAttachment(GetMesh(), BackSniperSocketName);

	ShotgunSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ShotgunSceneComponent"));
	ShotgunSceneComponent->SetupAttachment(GetMesh(), BackShotgunSocketName);

	GrenadeSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("GrenadeSceneComponent"));
	GrenadeSceneComponent->SetupAttachment(GetMesh(), GrenadeSocketName);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	FPSCameraSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("FPSCameraSceneComponent"));
	FPSCameraSceneComponent->SetupAttachment(RootComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void AMultiShootGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	ToggleDefaultAimWidget(true);

	HealthComponent->OnHealthChanged.AddDynamic(this, &AMultiShootGameCharacter::OnHealthChanged);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	SpawnActorLocation = GetActorLocation();

	CurrentWeapon = GetWorld()->SpawnActor<AMultiShootGameWeapon>(WeaponClass, FVector::ZeroVector,
	                                                              FRotator::ZeroRotator,
	                                                              SpawnParameters);
	CurrentSniper = GetWorld()->SpawnActor<AMultiShootGameWeapon>(SniperClass, FVector::ZeroVector,
	                                                              FRotator::ZeroRotator,
	                                                              SpawnParameters);
	CurrentShotgun = GetWorld()->SpawnActor<AMultiShootGameWeapon>(ShotgunClass, FVector::ZeroVector,
	                                                               FRotator::ZeroRotator,
	                                                               SpawnParameters);
	CurrentFPSCamera = GetWorld()->SpawnActor<AMultiShootGameFPSCamera>(FPSCameraClass, FVector::ZeroVector,
	                                                                    FRotator::ZeroRotator,
	                                                                    SpawnParameters);

	if (CurrentWeapon)
	{
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToComponent(WeaponSceneComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}

	if (CurrentSniper)
	{
		CurrentSniper->SetOwner(this);
		CurrentSniper->AttachToComponent(SniperSceneComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}

	if (CurrentShotgun)
	{
		CurrentShotgun->SetOwner(this);
		CurrentShotgun->AttachToComponent(ShotgunSceneComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}

	if (CurrentFPSCamera)
	{
		CurrentFPSCamera->SetOwner(this);
		CurrentFPSCamera->AttachToComponent(FPSCameraSceneComponent,
		                                    FAttachmentTransformRules::SnapToTargetIncludingScale);

		CurrentFPSCamera->SetActorHiddenInGame(true);
	}

	if (CurrentGrenade)
	{
		CurrentGrenade->SetOwner(this);
		CurrentGrenade->AttachToComponent(GrenadeSceneComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}

void AMultiShootGameCharacter::StartFire()
{
	if (!CheckStatus(false))
	{
		return;
	}

	bFired = true;

	if (!bAimed)
	{
		switch (WeaponMode)
		{
		case EWeaponMode::Weapon:
			if (CurrentWeapon)
			{
				CurrentWeapon->StartFire();
			}
			break;
		case EWeaponMode::Sniper:
			if (CurrentSniper)
			{
				CurrentSniper->Fire();
				BeginSniperReload();
			}
			break;
		case EWeaponMode::Shotgun:
			if (CurrentShotgun)
			{
				CurrentShotgun->ShotgunFire();
			}
			break;
		}
	}
	else
	{
		if (CurrentFPSCamera)
		{
			switch (WeaponMode)
			{
			case EWeaponMode::Weapon:
				CurrentFPSCamera->StartFire();
				break;
			case EWeaponMode::Sniper:
				CurrentFPSCamera->Fire();
				BeginSniperReload();
				break;
			case EWeaponMode::Shotgun:
				CurrentFPSCamera->ShotgunFire();
				break;
			}
		}
	}
}

void AMultiShootGameCharacter::StopFire()
{
	bFired = false;

	if (WeaponMode == EWeaponMode::Weapon)
	{
		if (CurrentWeapon)
		{
			CurrentWeapon->StopFire();
		}

		if (CurrentFPSCamera)
		{
			CurrentFPSCamera->StopFire();
		}
	}
}

void AMultiShootGameCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);

	if (bAimed && Value != 0)
	{
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->ClientStartCameraShake(MovementCameraShakeClass);
	}
}

void AMultiShootGameCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);

	if (bAimed && Value != 0)
	{
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->ClientStartCameraShake(MovementCameraShakeClass);
	}
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

void AMultiShootGameCharacter::ToggleCrouch()
{
	if (!GetCharacterMovement()->IsCrouching())
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}

void AMultiShootGameCharacter::BeginAim()
{
	if (!CheckStatus(false))
	{
		return;
	}

	bAimed = true;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->SetViewTargetWithBlend(CurrentFPSCamera, 0.1f);

	CurrentFPSCamera->SetActorHiddenInGame(false);
	CurrentWeapon->SetActorHiddenInGame(true);
	CurrentSniper->SetActorHiddenInGame(true);
	CurrentShotgun->SetActorHiddenInGame(true);
	GetMesh()->SetHiddenInGame(true);

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->ClientStartCameraShake(FPSCameraShakeClass);

	if (WeaponMode == EWeaponMode::Sniper)
	{
		ToggleSniperAimWidget(true);
		CurrentFPSCamera->SetZoomed(true);
	}

	if (bFired)
	{
		CurrentFPSCamera->StartFire();
	}

	CurrentWeapon->StopFire();
}

void AMultiShootGameCharacter::EndAim()
{
	bAimed = false;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->SetViewTargetWithBlend(this, 0.1f);

	CurrentFPSCamera->SetActorHiddenInGame(true);
	CurrentWeapon->SetActorHiddenInGame(false);
	CurrentSniper->SetActorHiddenInGame(false);
	CurrentShotgun->SetActorHiddenInGame(false);
	GetMesh()->SetHiddenInGame(false);

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->ClientStopCameraShake(FPSCameraShakeClass);

	if (WeaponMode == EWeaponMode::Sniper)
	{
		ToggleSniperAimWidget(false);
		CurrentFPSCamera->SetZoomed(false);
	}

	if (bFired)
	{
		if (WeaponMode == EWeaponMode::Weapon)
		{
			CurrentWeapon->StartFire();
		}
	}

	CurrentFPSCamera->StopFire();
}


void AMultiShootGameCharacter::BeginReload()
{
	if (!CheckStatus(false))
	{
		return;
	}

	bReloading = true;

	EndAction();

	PlayAnimMontage(ReloadAnimMontage);
}

void AMultiShootGameCharacter::BeginSniperReload()
{
	if (!CheckStatus(false))
	{
		return;
	}

	bReloading = true;

	EndAction();

	FLatentActionInfo LatentActionInfo;
	UKismetSystemLibrary::Delay(GetWorld(), 0.5f, LatentActionInfo);

	PlayAnimMontage(SniperReloadAnimMontage);
}

void AMultiShootGameCharacter::BeginThrowGrenade()
{
	if (!CheckStatus(false))
	{
		return;
	}

	if (bBeginThrowGrenade || bThrowingGrenade)
	{
		return;
	}

	EndAction();

	bBeginThrowGrenade = true;

	WeaponSceneComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
	                                        BackWeaponSocketName);

	SniperSceneComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
	                                        BackSniperSocketName);

	ShotgunSceneComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
	                                         BackShotgunSocketName);

	PlayAnimMontage(ThrowGrenadeAnimMontage);
}

void AMultiShootGameCharacter::EndThrowGrenade()
{
	PlayAnimMontage(WeaponOutAnimMontage);
}

void AMultiShootGameCharacter::ThrowGrenade()
{
	if (!CheckStatus(false))
	{
		return;
	}

	if (bThrowingGrenade)
	{
		return;
	}

	if (!bSpawnGrenade)
	{
		SpawnGrenade();
	}

	bThrowingGrenade = true;

	PlayAnimMontage(ThrowGrenadeAnimMontage, 1, FName("Throw"));
}

void AMultiShootGameCharacter::ThrowGrenadeOut()
{
	const FVector StartLocation = GrenadeSceneComponent->GetComponentLocation();

	const FVector CameraLocation = CameraComponent->GetComponentLocation();
	const FRotator CameraRotation = CameraComponent->GetComponentRotation();
	const FVector TargetLocation = CameraLocation + CameraRotation.Vector() * 3000.f;

	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);

	CurrentGrenade->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CurrentGrenade->ThrowGrenade(LookAtRotation);
}

void AMultiShootGameCharacter::SpawnGrenade()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CurrentGrenade = GetWorld()->SpawnActor<AMultiShootGameGrenade>(GrenadeClass, FVector::ZeroVector,
	                                                                FRotator::ZeroRotator, SpawnParameters);

	if (CurrentGrenade)
	{
		CurrentGrenade->SetOwner(this);
		CurrentGrenade->AttachToComponent(GrenadeSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}

	bSpawnGrenade = true;
}

void AMultiShootGameCharacter::EndReload()
{
	bReloading = false;
}

void AMultiShootGameCharacter::ToggleWeapon()
{
	if (!CheckStatus(true))
	{
		return;
	}

	if (WeaponMode == EWeaponMode::Weapon)
	{
		return;
	}

	bToggleWeapon = true;

	EndAction();

	WeaponMode = EWeaponMode::Weapon;

	PlayAnimMontage(WeaponOutAnimMontage);
}

void AMultiShootGameCharacter::ToggleSniper()
{
	if (!CheckStatus(true))
	{
		return;
	}

	if (WeaponMode == EWeaponMode::Sniper)
	{
		return;
	}

	bToggleWeapon = true;

	EndAction();

	ToggleSniperAimWidget(false);

	WeaponMode = EWeaponMode::Sniper;

	PlayAnimMontage(WeaponOutAnimMontage);
}

void AMultiShootGameCharacter::ToggleShotgun()
{
	if (!CheckStatus(true))
	{
		return;
	}

	if (WeaponMode == EWeaponMode::Shotgun)
	{
		return;
	}

	bToggleWeapon = true;

	EndAction();

	WeaponMode = EWeaponMode::Shotgun;

	PlayAnimMontage(WeaponOutAnimMontage);
}

void AMultiShootGameCharacter::ToggleWeaponBegin()
{
	FLatentActionInfo LatentActionInfo;
	LatentActionInfo.CallbackTarget = this;

	switch (WeaponMode)
	{
	case EWeaponMode::Weapon:
		WeaponSceneComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform,
		                                        WeaponSocketName);

		SniperSceneComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
		                                        BackSniperSocketName);

		ShotgunSceneComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
		                                         BackShotgunSocketName);

		UKismetSystemLibrary::MoveComponentTo(WeaponSceneComponent, FVector::ZeroVector, FRotator::ZeroRotator, true,
		                                      true, 0.2f, false, EMoveComponentAction::Type::Move, LatentActionInfo);

		CurrentFPSCamera->ToggleWeapon(EWeaponMode::Weapon);

		ToggleDefaultAimWidget(true);


		break;
	case EWeaponMode::Sniper:
		WeaponSceneComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
		                                        BackWeaponSocketName);

		SniperSceneComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform,
		                                        SniperSocketName);

		ShotgunSceneComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
		                                         BackShotgunSocketName);

		UKismetSystemLibrary::MoveComponentTo(SniperSceneComponent, FVector::ZeroVector, FRotator::ZeroRotator, true,
		                                      true, 0.2f, false, EMoveComponentAction::Type::Move, LatentActionInfo);

		CurrentFPSCamera->ToggleWeapon(EWeaponMode::Sniper);

		ToggleDefaultAimWidget(false);

		break;
	case EWeaponMode::Shotgun:
		WeaponSceneComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
		                                        BackWeaponSocketName);

		SniperSceneComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
		                                        BackWeaponSocketName);

		ShotgunSceneComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform,
		                                         ShotgunSocketName);

		UKismetSystemLibrary::MoveComponentTo(ShotgunSceneComponent, FVector::ZeroVector, FRotator::ZeroRotator, true,
		                                      true, 0.2f, false, EMoveComponentAction::Type::Move, LatentActionInfo);

		CurrentFPSCamera->ToggleWeapon(EWeaponMode::Shotgun);

		ToggleDefaultAimWidget(true);

		break;
	}
}

void AMultiShootGameCharacter::ToggleWeaponEnd()
{
	bToggleWeapon = false;

	bBeginThrowGrenade = false;

	bThrowingGrenade = false;

	bSpawnGrenade = false;
}

void AMultiShootGameCharacter::ToggleDefaultAimWidget(bool Enabled)
{
	if (Enabled)
	{
		CurrentDefaultUserWidget = CreateWidget(GetWorld(), DefaultUserWidgetClass);
		CurrentDefaultUserWidget->AddToViewport();
	}
	else
	{
		if (CurrentDefaultUserWidget)
		{
			CurrentDefaultUserWidget->RemoveFromViewport();
		}
	}
}

void AMultiShootGameCharacter::ToggleSniperAimWidget(bool Enabled)
{
	if (Enabled)
	{
		CurrentSniperUserWidget = CreateWidget(GetWorld(), SniperUserWidgetClass);
		CurrentSniperUserWidget->AddToViewport();
	}
	else
	{
		if (CurrentSniperUserWidget)
		{
			CurrentSniperUserWidget->RemoveFromViewport();
		}
	}
}

void AMultiShootGameCharacter::AimLookAround()
{
	const FVector StartLocation = FPSCameraSceneComponent->GetComponentLocation();

	const FVector CameraLocation = CameraComponent->GetComponentLocation();
	const FRotator CameraRotation = CameraComponent->GetComponentRotation();
	const FVector TargetLocation = CameraLocation + CameraRotation.Vector() * 3000.f;

	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);

	const FRotator TargetRotation = FRotator(0, LookAtRotation.Yaw - 90.f, LookAtRotation.Pitch * -1.f);

	FPSCameraSceneComponent->SetWorldRotation(TargetRotation);
}

bool AMultiShootGameCharacter::CheckStatus(bool checkAimed)
{
	if (HealthComponent->bDied || bReloading || bToggleWeapon)
	{
		return false;
	}

	if (checkAimed && bAimed)
	{
		return false;
	}

	return true;
}

void AMultiShootGameCharacter::EndAction()
{
	if (WeaponMode != EWeaponMode::Sniper)
	{
		if (bAimed)
		{
			EndAim();
		}
	}

	if (bFired)
	{
		StopFire();
	}
}

void AMultiShootGameCharacter::Death()
{
	HealthComponent->bDied = true;

	EndAction();

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMovementComponent()->SetActive(false);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetAllBodiesPhysicsBlendWeight(0.4f);
	GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
	GetMesh()->GetAnimInstance()->StopAllMontages(0);

	CurrentWeapon->EnablePhysicsSimulate();
	CurrentSniper->EnablePhysicsSimulate();
	CurrentShotgun->EnablePhysicsSimulate();
}

void AMultiShootGameCharacter::OnHealthChanged(UHealthComponent* OwningHealthComponent, float Health, float HealthDelta,
                                               const UDamageType* DamageType, AController* InstigatedBy,
                                               AActor* DamageCauser)
{
	if (Health <= 0.0f && !HealthComponent->bDied)
	{
		Death();
	}
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

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMultiShootGameCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMultiShootGameCharacter::StopFire);

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

	// Bind crouch events
	PlayerInputComponent->BindAction("ToggleCrouch", IE_Pressed, this, &AMultiShootGameCharacter::ToggleCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMultiShootGameCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMultiShootGameCharacter::EndCrouch);

	// Bind reloading events
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AMultiShootGameCharacter::BeginReload);

	// Bind toggle weapon events
	PlayerInputComponent->BindAction("Weapon", IE_Pressed, this, &AMultiShootGameCharacter::ToggleWeapon);
	PlayerInputComponent->BindAction("Sniper", IE_Pressed, this, &AMultiShootGameCharacter::ToggleSniper);
	PlayerInputComponent->BindAction("Shotgun", IE_Pressed, this, &AMultiShootGameCharacter::ToggleShotgun);

	//Bind throw grenade
	PlayerInputComponent->BindAction("ThrowGrenade", IE_Pressed, this, &AMultiShootGameCharacter::BeginThrowGrenade);
	PlayerInputComponent->BindAction("ThrowGrenade", IE_Released, this, &AMultiShootGameCharacter::ThrowGrenade);
}

USceneComponent* AMultiShootGameCharacter::GetFPSCameraSceneComponent() const
{
	return FPSCameraSceneComponent;
}

UCameraComponent* AMultiShootGameCharacter::GetCameraComponent() const
{
	return CameraComponent;
}

AMultiShootGameFPSCamera* AMultiShootGameCharacter::GetCurrentFPSCamera() const
{
	return CurrentFPSCamera;
}

bool AMultiShootGameCharacter::GetAimed() const
{
	return bAimed;
}

EWeaponMode AMultiShootGameCharacter::GetWeaponMode() const
{
	return WeaponMode;
}
