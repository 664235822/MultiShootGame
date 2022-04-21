// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiShootGameCharacter.h"
#include "AIController.h"
#include "MultiShootGameEnemyCharacter.h"
#include "MultiShootGame/MultiShootGame.h"
#include "MultiShootGame/Weapon/MultiShootGameProjectile.h"
#include "AnimGraphRuntime/Public/KismetAnimationLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

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

	KnifeSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("KnifeSkeletalMeshComponent"));
	KnifeSkeletalMeshComponent->SetupAttachment(GetMesh(), KnifeSocketName);
	KnifeSkeletalMeshComponent->SetVisibility(false);

	TakeDownKnifeSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(
		TEXT("TakeDownKnifeSkeletalMeshComponent"));
	TakeDownKnifeSkeletalMeshComponent->SetupAttachment(GetMesh(), TakeDownKnifeSocketName);
	TakeDownKnifeSkeletalMeshComponent->SetVisibility(false);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	FPSCameraSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("FPSCameraSceneComponent"));
	FPSCameraSceneComponent->SetupAttachment(RootComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	HitEffectComponent = CreateDefaultSubobject<UHitEffectComponent>(TEXT("HitEfectComponent"));
}

void AMultiShootGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentDefaultUserWidget = CreateWidget(GetWorld(), DefaultUserWidgetClass);
	CurrentDefaultUserWidget->AddToViewport();
	CurrentDefaultUserWidget->SetVisibility(ESlateVisibility::Visible);

	CurrentSniperUserWidget = CreateWidget(GetWorld(), SniperUserWidgetClass);
	CurrentSniperUserWidget->AddToViewport();
	CurrentSniperUserWidget->SetVisibility(ESlateVisibility::Hidden);

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

		CurrentFPSCamera->SetWeaponInfo(CurrentWeapon);
	}

	if (CurrentGrenade)
	{
		CurrentGrenade->SetOwner(this);
		CurrentGrenade->AttachToComponent(GrenadeSceneComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}

void AMultiShootGameCharacter::StartFire()
{
	if (!CheckStatus(false, true))
	{
		return;
	}

	bFired = true;

	ToggleUseControlRotation(true);

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

	ToggleUseControlRotation(false);

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
	if (!bEnableMovement)
	{
		return;
	}

	AddMovementInput(GetActorForwardVector() * Value);

	if (bAimed && Value != 0)
	{
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->ClientStartCameraShake(MovementCameraShakeClass);
	}
}

void AMultiShootGameCharacter::MoveRight(float Value)
{
	if (!bEnableMovement)
	{
		return;
	}

	AddMovementInput(GetActorRightVector() * Value);

	if (bAimed && Value != 0)
	{
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->ClientStartCameraShake(MovementCameraShakeClass);
	}
}

void AMultiShootGameCharacter::BeginFastRun()
{
	bFastRun = true;

	HandleWalkSpeed();
}

void AMultiShootGameCharacter::EndFastRun()
{
	bFastRun = false;

	HandleWalkSpeed();
}

void AMultiShootGameCharacter::BeginCrouch()
{
	Crouch();

	ToggleUseControlRotation(true);

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->ClientStopCameraShake(FPSCameraShakeClass);
}

void AMultiShootGameCharacter::EndCrouch()
{
	UnCrouch();

	ToggleUseControlRotation(false);

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->ClientStartCameraShake(FPSCameraShakeClass);
}

void AMultiShootGameCharacter::ToggleCrouch()
{
	if (!GetCharacterMovement()->IsCrouching())
	{
		BeginCrouch();
	}
	else
	{
		EndCrouch();
	}
}

void AMultiShootGameCharacter::BeginAim()
{
	if (!CheckStatus(false, true))
	{
		return;
	}

	bAimed = true;

	ToggleUseControlRotation(true);

	SpringArmComponent->SocketOffset = FVector::ZeroVector;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->SetViewTargetWithBlend(CurrentFPSCamera, 0.1f);

	CurrentFPSCamera->SetActorHiddenInGame(false);
	CurrentWeapon->SetActorHiddenInGame(true);
	CurrentSniper->SetActorHiddenInGame(true);
	CurrentShotgun->SetActorHiddenInGame(true);
	GetMesh()->SetHiddenInGame(true);

	if (!GetCharacterMovement()->IsCrouching())
	{
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->ClientStartCameraShake(FPSCameraShakeClass);
	}

	ToggleDefaultAimWidget(false);

	if (WeaponMode == EWeaponMode::Sniper && CurrentSniper->WeaponInfo.AimTexture)
	{
		ToggleSniperAimWidget(true);
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

	ToggleUseControlRotation(false);

	SpringArmComponent->SocketOffset = FVector(0, 90.f, 0);

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->SetViewTargetWithBlend(this, 0.1f);

	CurrentFPSCamera->SetActorHiddenInGame(true);
	CurrentWeapon->SetActorHiddenInGame(false);
	CurrentSniper->SetActorHiddenInGame(false);
	CurrentShotgun->SetActorHiddenInGame(false);
	GetMesh()->SetHiddenInGame(false);

	if (!GetCharacterMovement()->IsCrouching())
	{
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->ClientStopCameraShake(FPSCameraShakeClass);
	}

	if (!(WeaponMode == EWeaponMode::Sniper && CurrentSniper->WeaponInfo.AimTexture))
	{
		ToggleDefaultAimWidget(true);
	}

	if (CurrentSniper->WeaponInfo.AimTexture)
	{
		ToggleSniperAimWidget(false);
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
	if (!CheckStatus(false, true))
	{
		return;
	}

	bReloading = true;

	ToggleUseControlRotation(true);

	EndAction();

	PlayAnimMontage(ReloadAnimMontage);
}

void AMultiShootGameCharacter::BeginSniperReload()
{
	if (!CheckStatus(false, true))
	{
		return;
	}

	bSniperReloading = true;

	EndAction();

	const FLatentActionInfo LatentActionInfo;
	UKismetSystemLibrary::Delay(GetWorld(), 0.5f, LatentActionInfo);

	PlayAnimMontage(SniperReloadAnimMontage);
}

void AMultiShootGameCharacter::BeginThrowGrenade()
{
	if (!CheckStatus(false, false))
	{
		return;
	}

	if (bBeginThrowGrenade || bThrowingGrenade)
	{
		return;
	}

	ToggleUseControlRotation(true);

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
	bToggleWeapon = true;

	PlayAnimMontage(WeaponOutAnimMontage);

	ToggleUseControlRotation(false);
}

void AMultiShootGameCharacter::ThrowGrenade()
{
	if (!CheckStatus(false, false))
	{
		return;
	}

	if (bThrowingGrenade)
	{
		return;
	}

	EndAction();

	bThrowingGrenade = true;

	if (!bBeginThrowGrenade)
	{
		WeaponSceneComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
		                                        BackWeaponSocketName);

		SniperSceneComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
		                                        BackSniperSocketName);

		ShotgunSceneComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
		                                         BackShotgunSocketName);
	}

	if (!bSpawnGrenade)
	{
		SpawnGrenade();
	}

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
	CurrentGrenade->ThrowGrenade(LookAtRotation, bFastRun || bJump);
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

	bBeginThrowGrenade = true;

	bSpawnGrenade = true;
}

void AMultiShootGameCharacter::KnifeAttack()
{
	if (!CheckStatus(false, true))
	{
		return;
	}

	EndAction();

	if (!bTakeDown)
	{
		if (!bNextKnifeAttack)
		{
			if (!bKnifeAttack)
			{
				PlayAnimMontage(KnifeAttackAnimMontage[0], 1.5f);
			}
		}
		else
		{
			PlayAnimMontage(KnifeAttackAnimMontage[KnifeComboIndex], 1.5f);
		}
	}
	else
	{
		FVector ActorLocation = GetActorLocation();
		FVector ForwardVector = GetActorForwardVector() * 250.0f;
		FVector EndLocation = ActorLocation + ForwardVector;

		TArray<AActor*> IgnoreActor;
		FHitResult HitResult;
		if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), ActorLocation, EndLocation,TraceType_Weapon, false,
		                                          IgnoreActor, EDrawDebugTrace::None, HitResult, true))
		{
			TargetTakeDownCharacter = Cast<AMultiShootGameEnemyCharacter>(
				HitResult.GetActor());
			if (TargetTakeDownCharacter)
			{
				SpringArmComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
				                                      FName("Spine1"));
				GetCharacterMovement()->DisableMovement();
				bTakeDown = false;
				bTakingDown = true;

				FTransform TargetTransform = TargetTakeDownCharacter->GetActorTransform();
				FVector TargetLocation = TargetTransform.GetLocation() + TargetTakeDownCharacter->
					GetActorForwardVector() * -80.f;
				FQuat TargetRotation = TargetTransform.GetRotation();
				SetActorTransform(FTransform(TargetRotation, TargetLocation));

				PlayAnimMontage(TakeDownAttackerAnimMontage);
			}
		}
	}
}

void AMultiShootGameCharacter::BeginKnifeAttack()
{
	bKnifeAttack = true;

	bNextKnifeAttack = false;

	WeaponSceneComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
	                                        BackWeaponSocketName);

	SniperSceneComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
	                                        BackSniperSocketName);

	ShotgunSceneComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
	                                         BackShotgunSocketName);

	USkeletalMeshComponent* CurrentKnifeComponent = bTakingDown
		                                                ? TakeDownKnifeSkeletalMeshComponent
		                                                : KnifeSkeletalMeshComponent;
	CurrentKnifeComponent->SetVisibility(true);
}

void AMultiShootGameCharacter::EndKnifeAttack()
{
	if (bTakingDown)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		SpringArmComponent->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		SpringArmComponent->SetRelativeLocation(FVector(0, 0, 70.f));

		TargetTakeDownCharacter = nullptr;
		bTakeDown = false;
		bTakingDown = false;
	}

	bKnifeAttack = false;

	bNextKnifeAttack = false;

	bToggleWeapon = true;

	KnifeComboIndex = 0;

	KnifeSkeletalMeshComponent->SetVisibility(false);

	TakeDownKnifeSkeletalMeshComponent->SetVisibility(false);

	PlayAnimMontage(WeaponOutAnimMontage);
}

void AMultiShootGameCharacter::NextKnifeAttack()
{
	bNextKnifeAttack = true;

	if (KnifeComboIndex < KnifeAttackAnimMontage.Num() - 1)
	{
		KnifeComboIndex++;
	}
	else
	{
		KnifeComboIndex = 0;
	}
}

void AMultiShootGameCharacter::TakeDownAttack()
{
	if (TargetTakeDownCharacter)
	{
		UGameplayStatics::ApplyDamage(TargetTakeDownCharacter, TakeDownDamage, GetInstigatorController(), this,
		                              DamageTypeClass);
		HitEffectComponent->PlayHitEffect(
			SURFACE_CHARACTER, TakeDownKnifeSkeletalMeshComponent->GetSocketLocation(HitSocketName),
			TakeDownKnifeSkeletalMeshComponent->GetComponentRotation());
	}
}

void AMultiShootGameCharacter::EndReload()
{
	bReloading = false;

	bSniperReloading = false;

	ToggleUseControlRotation(false);
}

void AMultiShootGameCharacter::ReloadShowClip(bool Enabled)
{
	AMultiShootGameWeapon* TempWeapon = nullptr;
	switch (WeaponMode)
	{
	case EWeaponMode::Weapon:
		TempWeapon = CurrentWeapon;
		break;
	case EWeaponMode::Sniper:
		TempWeapon = CurrentSniper;
		break;
	case EWeaponMode::Shotgun:
		TempWeapon = CurrentShotgun;
		break;
	}

	TempWeapon->ReloadShowClip(Enabled);
}

void AMultiShootGameCharacter::ToggleWeapon()
{
	if (!CheckStatus(true, true))
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

	CurrentFPSCamera->SetWeaponInfo(CurrentWeapon);

	bUseControllerRotationYaw = false;

	HandleWalkSpeed();

	PlayAnimMontage(WeaponOutAnimMontage);
}

void AMultiShootGameCharacter::ToggleSniper()
{
	if (!CheckStatus(true, true))
	{
		return;
	}

	if (WeaponMode == EWeaponMode::Sniper)
	{
		return;
	}

	bToggleWeapon = true;

	EndAction();

	WeaponMode = EWeaponMode::Sniper;

	CurrentFPSCamera->SetWeaponInfo(CurrentSniper);

	bUseControllerRotationYaw = true;

	HandleWalkSpeed();

	PlayAnimMontage(WeaponOutAnimMontage);
}

void AMultiShootGameCharacter::ToggleShotgun()
{
	if (!CheckStatus(true, true))
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

	CurrentFPSCamera->SetWeaponInfo(CurrentShotgun);

	bUseControllerRotationYaw = true;

	HandleWalkSpeed();

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

		break;
	}

	ToggleDefaultAimWidget(!(WeaponMode == EWeaponMode::Sniper && CurrentSniper->WeaponInfo.AimTexture));
}

void AMultiShootGameCharacter::ToggleWeaponEnd()
{
	bToggleWeapon = false;

	bBeginThrowGrenade = false;

	bThrowingGrenade = false;

	bSpawnGrenade = false;
}

void AMultiShootGameCharacter::Hit()
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->ClientStartCameraShake(KnifeCameraShakeClass);

	const FVector HitLocation = KnifeSkeletalMeshComponent->GetSocketLocation(HitSocketName);
	const FRotator HitRotation = KnifeSkeletalMeshComponent->GetComponentRotation();
	FHitResult HitResult;
	const TArray<AActor*> IgnoreActors;
	if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), HitLocation, HitLocation, 50.f, TraceType_Weapon, false,
	                                            IgnoreActors, EDrawDebugTrace::None, HitResult, true))
	{
		const EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());

		UGameplayStatics::ApplyDamage(HitResult.GetActor(), KnifeDamage, GetInstigatorController(), this,
		                              DamageTypeClass);

		HitEffectComponent->PlayHitEffect(SurfaceType, HitLocation, HitRotation);
	}
}

void AMultiShootGameCharacter::ToggleDefaultAimWidget(bool Enabled)
{
	CurrentDefaultUserWidget->SetVisibility(Enabled ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void AMultiShootGameCharacter::ToggleSniperAimWidget(bool Enabled)
{
	CurrentSniperUserWidget->SetVisibility(Enabled ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
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

bool AMultiShootGameCharacter::CheckStatus(bool CheckAimed, bool CheckThrowGrenade)
{
	if (HealthComponent->bDied || bReloading || bToggleWeapon || bSniperReloading || bThrowingGrenade || bTakingDown)
	{
		return false;
	}

	if (CheckAimed && bAimed)
	{
		return false;
	}

	if (CheckThrowGrenade && bBeginThrowGrenade)
	{
		return false;
	}

	if (bKnifeAttack && !bNextKnifeAttack)
	{
		return false;
	}

	return true;
}

void AMultiShootGameCharacter::EndAction()
{
	if (bAimed && !bSniperReloading)
	{
		EndAim();
	}

	if (bFired)
	{
		StopFire();
	}
}

void AMultiShootGameCharacter::HandleWalkSpeed()
{
	if (bFastRun)
	{
		GetCharacterMovement()->MaxWalkSpeed = WeaponMode != EWeaponMode::Sniper ? 600.f : 500.f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WeaponMode != EWeaponMode::Sniper ? 300.f : 250.f;
	}
}

void AMultiShootGameCharacter::ToggleUseControlRotation(bool Enabled)
{
	if (WeaponMode == EWeaponMode::Weapon)
	{
		bUseControlRotation = Enabled;
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

	if (WeaponMode == EWeaponMode::Weapon && (bUseControlRotation ||
		(!bUseControlRotation && GetMovementComponent()->Velocity.Size() != 0)))
	{
		const FRotator TargetRotation = FRotator(0, GetControlRotation().Yaw, 0);
		const FRotator Rotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 15);
		SetActorRotation(Rotation);
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

	// Bind throw grenade
	PlayerInputComponent->BindAction("ThrowGrenade", IE_Pressed, this, &AMultiShootGameCharacter::BeginThrowGrenade);
	PlayerInputComponent->BindAction("ThrowGrenade", IE_Released, this, &AMultiShootGameCharacter::ThrowGrenade);

	// Bind knife attack
	PlayerInputComponent->BindAction("KnifeAttack", IE_Pressed, this, &AMultiShootGameCharacter::KnifeAttack);
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

void AMultiShootGameCharacter::SetTakeDown(bool Value)
{
	bTakeDown = Value;
}
