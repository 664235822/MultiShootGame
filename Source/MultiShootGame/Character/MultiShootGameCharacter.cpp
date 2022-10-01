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
#include "Net/UnrealNetwork.h"

AMultiShootGameCharacter::AMultiShootGameCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bFindCameraComponentWhenViewTarget = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);

	MainWeaponSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MainWeaponSceneComponent"));
	MainWeaponSceneComponent->SetupAttachment(GetMesh(), MainWeaponSocketName);
	MainWeaponSceneComponent->SetIsReplicated(true);

	SecondWeaponSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SecondWeaponSceneComponent"));
	SecondWeaponSceneComponent->SetupAttachment(GetMesh(), BackSecondWeaponSocketName);
	SecondWeaponSceneComponent->SetIsReplicated(true);

	ThirdWeaponSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ThirdWeaponSceneComponent"));
	ThirdWeaponSceneComponent->SetupAttachment(GetMesh(), BackThirdWeaponSocketName);
	ThirdWeaponSceneComponent->SetIsReplicated(true);

	GrenadeSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("GrenadeSceneComponent"));
	GrenadeSceneComponent->SetupAttachment(GetMesh(), GrenadeSocketName);

	KnifeSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("KnifeSkeletalMeshComponent"));
	KnifeSkeletalMeshComponent->SetupAttachment(GetMesh(), KnifeSocketName);
	KnifeSkeletalMeshComponent->SetVisibility(false);
	KnifeSkeletalMeshComponent->SetIsReplicated(true);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	FPSCameraSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("FPSCameraSceneComponent"));
	FPSCameraSceneComponent->SetupAttachment(RootComponent);

	DeathAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("DeathAudioComponent"));
	DeathAudioComponent->SetupAttachment(RootComponent);
	DeathAudioComponent->SetAutoActivate(false);

	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	if (GetLocalRole() == ROLE_Authority)
	{
		HealthComponent->OnHealthChanged.AddDynamic(this, &AMultiShootGameCharacter::OnHealthChanged);
	}

	HitEffectComponent = CreateDefaultSubobject<UHitEffectComponent>(TEXT("HitEfectComponent"));
}

void AMultiShootGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	PlayerCameraManager->ViewPitchMax = CameraPitchClamp;
	PlayerCameraManager->ViewPitchMin = -1 * CameraPitchClamp;

	GrenadeCount = MaxGrenadeCount;

	if (IsLocallyControlled())
	{
		CurrentSniperUserWidget = CreateWidget(GetWorld(), SniperUserWidgetClass);
		CurrentSniperUserWidget->AddToViewport();
		CurrentSniperUserWidget->SetVisibility(ESlateVisibility::Hidden);

		CurrentGameUserWidget = CreateWidget(GetWorld(), GameUserWidgetClass);
		CurrentGameUserWidget->AddToViewport();
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CurrentMainWeapon = GetWorld()->SpawnActor<AMultiShootGameWeapon>(MainWeaponClass, FVector::ZeroVector,
	                                                                  FRotator::ZeroRotator,
	                                                                  SpawnParameters);
	CurrentSecondWeapon = GetWorld()->SpawnActor<AMultiShootGameWeapon>(SecondWeaponClass, FVector::ZeroVector,
	                                                                    FRotator::ZeroRotator,
	                                                                    SpawnParameters);
	CurrentThirdWeapon = GetWorld()->SpawnActor<AMultiShootGameWeapon>(ThirdWeaponClass, FVector::ZeroVector,
	                                                                   FRotator::ZeroRotator,
	                                                                   SpawnParameters);
	CurrentFPSCamera = GetWorld()->SpawnActor<AMultiShootGameFPSCamera>(FPSCameraClass, FVector::ZeroVector,
	                                                                    FRotator::ZeroRotator,
	                                                                    SpawnParameters);

	if (CurrentMainWeapon)
	{
		CurrentMainWeapon->SetOwner(this);
		CurrentMainWeapon->AttachToComponent(MainWeaponSceneComponent,
		                                     FAttachmentTransformRules::SnapToTargetIncludingScale);
	}

	if (CurrentSecondWeapon)
	{
		CurrentSecondWeapon->SetOwner(this);
		CurrentSecondWeapon->AttachToComponent(SecondWeaponSceneComponent,
		                                       FAttachmentTransformRules::SnapToTargetIncludingScale);
	}

	if (CurrentThirdWeapon)
	{
		CurrentThirdWeapon->SetOwner(this);
		CurrentThirdWeapon->AttachToComponent(ThirdWeaponSceneComponent,
		                                      FAttachmentTransformRules::SnapToTargetIncludingScale);
	}

	if (CurrentFPSCamera)
	{
		CurrentFPSCamera->SetOwner(this);
		CurrentFPSCamera->AttachToComponent(FPSCameraSceneComponent,
		                                    FAttachmentTransformRules::SnapToTargetIncludingScale);

		CurrentFPSCamera->SetActorHiddenInGame(true);

		CurrentFPSCamera->SetWeaponInfo(CurrentMainWeapon);
	}

	if (CurrentGrenade)
	{
		CurrentGrenade->SetOwner(this);
		CurrentGrenade->AttachToComponent(GrenadeSceneComponent,
		                                  FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}

void AMultiShootGameCharacter::Destroyed()
{
	if (!CurrentMainWeapon || !CurrentSecondWeapon || !CurrentThirdWeapon || !CurrentFPSCamera)
	{
		return;
	}

	CurrentMainWeapon->Destroy();
	CurrentSecondWeapon->Destroy();
	CurrentThirdWeapon->Destroy();
	CurrentFPSCamera->Destroy();

	Super::Destroyed();
}

void AMultiShootGameCharacter::StartFire()
{
	if (!CheckStatus(false, true))
	{
		return;
	}

	bFired = true;

	if (!bAimed)
	{
		switch (WeaponMode)
		{
		case EWeaponMode::MainWeapon:
			if (CurrentMainWeapon)
			{
				CurrentMainWeapon->StartFire();
			}
			break;
		case EWeaponMode::SecondWeapon:
			if (CurrentSecondWeapon)
			{
				CurrentSecondWeapon->Fire();
				BeginSecondWeaponReload();
			}
			break;
		case EWeaponMode::ThirdWeapon:
			if (CurrentThirdWeapon)
			{
				CurrentThirdWeapon->FireOfDelay();
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
			case EWeaponMode::MainWeapon:
				CurrentFPSCamera->StartFire();
				break;
			case EWeaponMode::SecondWeapon:
				CurrentFPSCamera->Fire();
				BeginSecondWeaponReload();
				break;
			case EWeaponMode::ThirdWeapon:
				CurrentFPSCamera->FireOfDelay();
				break;
			}
		}
	}
}

void AMultiShootGameCharacter::StopFire()
{
	bFired = false;

	if (WeaponMode == EWeaponMode::MainWeapon)
	{
		if (CurrentMainWeapon)
		{
			CurrentMainWeapon->StopFire();
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

	if (Value != 0)
	{
		if (bAimed)
		{
			Cast<APlayerController>(GetController())->ClientStartCameraShake(MovementCameraShakeClass);
		}
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
		Cast<APlayerController>(GetController())->ClientStartCameraShake(MovementCameraShakeClass);
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

	Cast<APlayerController>(GetController())->ClientStopCameraShake(FPSCameraShakeClass);
}

void AMultiShootGameCharacter::EndCrouch()
{
	UnCrouch();

	Cast<APlayerController>(GetController())->ClientStartCameraShake(FPSCameraShakeClass);
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

	SpringArmComponent->SocketOffset = FVector::ZeroVector;

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	PlayerController->SetViewTargetWithBlend(CurrentFPSCamera, 0.1f);

	CurrentFPSCamera->SetActorHiddenInGame(false);
	CurrentMainWeapon->SetActorHiddenInGame(true);
	CurrentSecondWeapon->SetActorHiddenInGame(true);
	CurrentThirdWeapon->SetActorHiddenInGame(true);
	GetMesh()->SetHiddenInGame(true);

	if (!GetCharacterMovement()->IsCrouching())
	{
		Cast<APlayerController>(GetController())->ClientStartCameraShake(FPSCameraShakeClass);
	}

	if (WeaponMode == EWeaponMode::SecondWeapon && CurrentSecondWeapon->WeaponInfo.AimTexture)
	{
		CurrentSniperUserWidget->SetVisibility(ESlateVisibility::Visible);
	}

	if (WeaponMode == EWeaponMode::MainWeapon && bFired)
	{
		CurrentFPSCamera->StartFire();
	}

	CurrentMainWeapon->StopFire();
}

void AMultiShootGameCharacter::EndAim()
{
	bAimed = false;

	SpringArmComponent->SocketOffset = FVector(0, 90.f, 0);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	PlayerController->SetViewTargetWithBlend(this, 0.1f);

	CurrentFPSCamera->SetActorHiddenInGame(true);
	CurrentMainWeapon->SetActorHiddenInGame(false);
	CurrentSecondWeapon->SetActorHiddenInGame(false);
	CurrentThirdWeapon->SetActorHiddenInGame(false);
	GetMesh()->SetHiddenInGame(false);

	if (!GetCharacterMovement()->IsCrouching())
	{
		Cast<APlayerController>(GetController())->ClientStopCameraShake(FPSCameraShakeClass);
	}

	if (CurrentSecondWeapon->WeaponInfo.AimTexture)
	{
		CurrentSniperUserWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (WeaponMode == EWeaponMode::MainWeapon && bFired)
	{
		CurrentMainWeapon->StartFire();
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

	EndAction();

	switch (WeaponMode)
	{
	case EWeaponMode::MainWeapon:
		if (CurrentMainWeapon->WeaponInfo.MaxBulletNumber > 0)
		{
			PlayAnimMontage_Server(ReloadAnimMontage);
		}
	case EWeaponMode::SecondWeapon:
		if (CurrentSecondWeapon->WeaponInfo.MaxBulletNumber > 0)
		{
			PlayAnimMontage_Server(ReloadAnimMontage);
		}
		break;
	case EWeaponMode::ThirdWeapon:
		if (CurrentThirdWeapon->WeaponInfo.MaxBulletNumber > 0)
		{
			PlayAnimMontage_Server(ThirdWeaponReloadAnimMontage);
		}
		break;
	}
}

void AMultiShootGameCharacter::BeginSecondWeaponReload()
{
	if (!CheckStatus(false, true))
	{
		return;
	}

	bSecondWeaponReloading = true;

	EndAction();

	const FLatentActionInfo LatentActionInfo;
	UKismetSystemLibrary::Delay(GetWorld(), 0.5f, LatentActionInfo);

	PlayAnimMontage_Server(SecondWeaponReloadAnimMontage);
}

void AMultiShootGameCharacter::BeginThrowGrenade()
{
	if (!CheckStatus(false, false))
	{
		return;
	}

	if (bBeginThrowGrenade || bThrowingGrenade || GrenadeCount == 0)
	{
		return;
	}

	EndAction();

	SetBeginThrowGrenade_Server(true);

	PutBackWeapon_Server();

	PlayAnimMontage_Server(ThrowGrenadeAnimMontage);
}

void AMultiShootGameCharacter::EndThrowGrenade_Server_Implementation()
{
	if (bBeginThrowGrenade || bThrowingGrenade)
	{
		bToggleWeapon = true;

		PlayAnimMontage_Server(WeaponOutAnimMontage);
	}
}

void AMultiShootGameCharacter::ThrowGrenade()
{
	if (!CheckStatus(false, false))
	{
		return;
	}

	if (bThrowingGrenade || GrenadeCount == 0)
	{
		return;
	}

	EndAction();

	SetThrowingGrenade_Server(true);

	if (!bBeginThrowGrenade)
	{
		PutBackWeapon_Server();
	}

	if (!bSpawnGrenade)
	{
		SpawnGrenade();
	}

	PlayAnimMontage_Server(ThrowGrenadeAnimMontage, 1, FName("Throw"));
}

void AMultiShootGameCharacter::ThrowGrenadeOut()
{
	if (bSpawnGrenade && CurrentGrenade)
	{
		const FVector StartLocation = GrenadeSceneComponent->GetComponentLocation();

		const FVector CameraLocation = CameraComponent->GetComponentLocation();
		const FRotator CameraRotation = CameraComponent->GetComponentRotation();
		const FVector TargetLocation = CameraLocation + CameraRotation.Vector() * 3000.f;

		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);

		ThrowGrenadeOut_Server(LookAtRotation, bFastRun || bJump);

		GrenadeCount = FMath::Clamp(GrenadeCount - 1, 0, MaxGrenadeCount);
	}
}

void AMultiShootGameCharacter::ThrowGrenadeOut_Server_Implementation(FRotator Direction, bool MultiThrow)
{
	CurrentGrenade->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CurrentGrenade->ThrowGrenade_Server(Direction, MultiThrow);
}

void AMultiShootGameCharacter::SpawnGrenade()
{
	SpawnGrenade_Server();

	SetBeginThrowGrenade_Server(true);
	SetSpawnGrenade_Server(true);
}

void AMultiShootGameCharacter::SpawnGrenade_Server_Implementation()
{
	if (bBeginThrowGrenade || bThrowingGrenade)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		CurrentGrenade = GetWorld()->SpawnActor<AMultiShootGameGrenade>(GrenadeClass, FVector::ZeroVector,
		                                                                FRotator::ZeroRotator, SpawnParameters);

		if (CurrentGrenade)
		{
			CurrentGrenade->BaseDamage = GrenadeDamage;
			CurrentGrenade->SetOwner(this);
			CurrentGrenade->AttachToComponent(GrenadeSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
}

void AMultiShootGameCharacter::KnifeAttack()
{
	if (!CheckStatus(false, true))
	{
		return;
	}

	EndAction();

	SetKnifeAttack_Server(true);

	PlayAnimMontage_Server(KnifeAttackAnimMontage, 2.0f);
}

void AMultiShootGameCharacter::BeginKnifeAttack_Server_Implementation()
{
	if (bKnifeAttack)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_None);

		KnifeSkeletalMeshComponent->SetVisibility(true);

		PutBackWeapon_Server();
	}
}

void AMultiShootGameCharacter::EndKnifeAttack_Server_Implementation()
{
	if (bKnifeAttack)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);

		bToggleWeapon = true;

		KnifeSkeletalMeshComponent->SetVisibility(false);

		PlayAnimMontage_Server(WeaponOutAnimMontage);
	}
}

void AMultiShootGameCharacter::EndReload()
{
	bReloading = false;

	if (!bSecondWeaponReloading)
	{
		switch (WeaponMode)
		{
		case EWeaponMode::MainWeapon:
			if (CurrentMainWeapon->WeaponInfo.BulletNumber < CurrentMainWeapon->WeaponInfo.FillUpBulletNumber)
			{
				CurrentMainWeapon->BulletReload();
			}
			break;
		case EWeaponMode::SecondWeapon:
			if (CurrentSecondWeapon->WeaponInfo.BulletNumber < CurrentSecondWeapon->WeaponInfo.FillUpBulletNumber)
			{
				CurrentSecondWeapon->BulletReload();
			}
			break;
		case EWeaponMode::ThirdWeapon:
			if (CurrentThirdWeapon->WeaponInfo.BulletNumber < CurrentThirdWeapon->WeaponInfo.FillUpBulletNumber)
			{
				CurrentThirdWeapon->BulletReload();
			}
			break;
		}
	}

	bSecondWeaponReloading = false;
}

void AMultiShootGameCharacter::ReloadShowClip(bool Enabled)
{
	AMultiShootGameWeapon* TempWeapon = nullptr;
	switch (WeaponMode)
	{
	case EWeaponMode::MainWeapon:
		TempWeapon = CurrentMainWeapon;
		break;
	case EWeaponMode::SecondWeapon:
		TempWeapon = CurrentSecondWeapon;
		break;
	case EWeaponMode::ThirdWeapon:
		TempWeapon = CurrentThirdWeapon;
		break;
	}

	TempWeapon->ReloadShowMagazineClip(Enabled);
}

void AMultiShootGameCharacter::ToggleMainWeapon()
{
	if (!CheckStatus(true, true))
	{
		return;
	}

	if (WeaponMode == EWeaponMode::MainWeapon)
	{
		return;
	}

	bToggleWeapon = true;

	EndAction();

	SetWeaponMode_Server(EWeaponMode::MainWeapon);

	CurrentFPSCamera->SetWeaponInfo(CurrentMainWeapon);

	HandleWalkSpeed();

	PlayAnimMontage_Server(WeaponOutAnimMontage);
}

void AMultiShootGameCharacter::ToggleSecondWeapon()
{
	if (!CheckStatus(true, true))
	{
		return;
	}

	if (WeaponMode == EWeaponMode::SecondWeapon)
	{
		return;
	}

	bToggleWeapon = true;

	EndAction();

	SetWeaponMode_Server(EWeaponMode::SecondWeapon);

	CurrentFPSCamera->SetWeaponInfo(CurrentSecondWeapon);

	HandleWalkSpeed();

	PlayAnimMontage_Server(WeaponOutAnimMontage);
}

void AMultiShootGameCharacter::ToggleThirdWeapon()
{
	if (!CheckStatus(true, true))
	{
		return;
	}

	if (WeaponMode == EWeaponMode::ThirdWeapon)
	{
		return;
	}

	bToggleWeapon = true;

	EndAction();

	SetWeaponMode_Server(EWeaponMode::ThirdWeapon);

	CurrentFPSCamera->SetWeaponInfo(CurrentThirdWeapon);

	HandleWalkSpeed();

	PlayAnimMontage_Server(WeaponOutAnimMontage);
}

void AMultiShootGameCharacter::ToggleWeaponUp()
{
	switch (WeaponMode)
	{
	case EWeaponMode::MainWeapon:
		ToggleThirdWeapon();
		break;
	case EWeaponMode::SecondWeapon:
		ToggleMainWeapon();
		break;
	case EWeaponMode::ThirdWeapon:
		ToggleSecondWeapon();
		break;
	}
}

void AMultiShootGameCharacter::ToggleWeaponDown()
{
	switch (WeaponMode)
	{
	case EWeaponMode::MainWeapon:
		ToggleSecondWeapon();
		break;
	case EWeaponMode::SecondWeapon:
		ToggleThirdWeapon();
		break;
	case EWeaponMode::ThirdWeapon:
		ToggleMainWeapon();
		break;
	}
}

void AMultiShootGameCharacter::ToggleWeaponBegin()
{
	AttachWeapon_Server();
}

void AMultiShootGameCharacter::ToggleWeaponEnd()
{
	bToggleWeapon = false;

	SetBeginThrowGrenade_Server(false);
	SetThrowingGrenade_Server(false);
	SetSpawnGrenade_Server(false);
	SetKnifeAttack_Server(false);
}

void AMultiShootGameCharacter::Hit()
{
	if (bKnifeAttack)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController)
		{
			PlayerController->ClientStartCameraShake(KnifeCameraShakeClass);
		}

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
}

void AMultiShootGameCharacter::FillUpWeaponBullet()
{
	CurrentMainWeapon->FillUpBullet();
	CurrentSecondWeapon->FillUpBullet();
	CurrentThirdWeapon->FillUpBullet();
	CurrentFPSCamera->FillUpBullet();
	GrenadeCount = MaxGrenadeCount;
}

bool AMultiShootGameCharacter::CheckStatus(bool CheckAimed, bool CheckThrowGrenade)
{
	if (HealthComponent->bDied || bDetectingClimb || bReloading || bToggleWeapon || bSecondWeaponReloading ||
		bThrowingGrenade || bKnifeAttack)
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

	return true;
}

void AMultiShootGameCharacter::EndAction()
{
	if (bAimed && !bSecondWeaponReloading)
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
	float Speed = 300.f;
	if (bFastRun)
	{
		if (WeaponMode != EWeaponMode::SecondWeapon)
		{
			Speed = 600.f;
		}
		else
		{
			Speed = 500.f;
		}
	}
	else
	{
		if (WeaponMode == EWeaponMode::SecondWeapon)
		{
			Speed = 250.F;
		}
	}

	GetCharacterMovement()->MaxWalkSpeed = Speed;
	SetWalkSpeed_Server(Speed);
}

void AMultiShootGameCharacter::SetWalkSpeed_Server_Implementation(float Value)
{
	GetCharacterMovement()->MaxWalkSpeed = Value;
}

void AMultiShootGameCharacter::SetWeaponMode_Server_Implementation(EWeaponMode Value)
{
	WeaponMode = Value;
}

void AMultiShootGameCharacter::SetBeginThrowGrenade_Server_Implementation(bool Value)
{
	bBeginThrowGrenade = Value;
}

void AMultiShootGameCharacter::SetThrowingGrenade_Server_Implementation(bool Value)
{
	bThrowingGrenade = Value;
}

void AMultiShootGameCharacter::SetSpawnGrenade_Server_Implementation(bool Value)
{
	bSpawnGrenade = Value;
}

void AMultiShootGameCharacter::SetKnifeAttack_Server_Implementation(bool Value)
{
	bKnifeAttack = Value;
}

void AMultiShootGameCharacter::AttachWeapon_Server_Implementation()
{
	FLatentActionInfo LatentActionInfo;
	LatentActionInfo.CallbackTarget = this;

	switch (WeaponMode)
	{
	case EWeaponMode::MainWeapon:
		MainWeaponSceneComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
		                                            MainWeaponSocketName);
		SecondWeaponSceneComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
		                                              BackSecondWeaponSocketName);
		ThirdWeaponSceneComponent->AttachToComponent(
			GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, BackThirdWeaponSocketName);

		UKismetSystemLibrary::MoveComponentTo(MainWeaponSceneComponent, FVector::ZeroVector, FRotator::ZeroRotator,
		                                      true, true, 0.2f, false, EMoveComponentAction::Type::Move,
		                                      LatentActionInfo);

		break;
	case EWeaponMode::SecondWeapon:
		MainWeaponSceneComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
		                                            BackMainWeaponSocketName);
		SecondWeaponSceneComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
		                                              SecondWeaponSocketName);
		ThirdWeaponSceneComponent->AttachToComponent(
			GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, BackThirdWeaponSocketName);

		UKismetSystemLibrary::MoveComponentTo(SecondWeaponSceneComponent, FVector::ZeroVector, FRotator::ZeroRotator,
		                                      true, true, 0.2f, false, EMoveComponentAction::Type::Move,
		                                      LatentActionInfo);

		break;
	case EWeaponMode::ThirdWeapon:
		MainWeaponSceneComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
		                                            BackMainWeaponSocketName);
		SecondWeaponSceneComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
		                                              BackSecondWeaponSocketName);
		ThirdWeaponSceneComponent->AttachToComponent(
			GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ThirdWeaponSocketName);

		UKismetSystemLibrary::MoveComponentTo(ThirdWeaponSceneComponent, FVector::ZeroVector, FRotator::ZeroRotator,
		                                      true, true, 0.2f, false, EMoveComponentAction::Type::Move,
		                                      LatentActionInfo);

		break;
	}
}

void AMultiShootGameCharacter::PutBackWeapon_Server_Implementation()
{
	MainWeaponSceneComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
	                                            BackMainWeaponSocketName);
	SecondWeaponSceneComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
	                                              BackSecondWeaponSocketName);
	ThirdWeaponSceneComponent->AttachToComponent(
		GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, BackThirdWeaponSocketName);
}

void AMultiShootGameCharacter::PlayAnimMontage_Server_Implementation(UAnimMontage* AnimMontage, float InPlayRate,
                                                                     FName StartSectionName)
{
	PlayAnimMontage_Multicast(AnimMontage, InPlayRate, StartSectionName);
}

void AMultiShootGameCharacter::PlayAnimMontage_Multicast_Implementation(UAnimMontage* AnimMontage,
                                                                        float InPlayRate, FName StartSectionName)
{
	PlayAnimMontage(AnimMontage, InPlayRate, StartSectionName);
}

void AMultiShootGameCharacter::Death()
{
	HealthComponent->bDied = true;

	EndAction();

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMovementComponent()->SetActive(false);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetAllBodiesPhysicsBlendWeight(DeathRagdollWeight);
	GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
	GetMesh()->GetAnimInstance()->StopAllMontages(0);

	CurrentMainWeapon->EnablePhysicsSimulate();
	CurrentSecondWeapon->EnablePhysicsSimulate();
	CurrentThirdWeapon->EnablePhysicsSimulate();

	DeathAudioComponent->Play();
}

void AMultiShootGameCharacter::OnHealthChanged(UHealthComponent* OwningHealthComponent, float Health, float HealthDelta,
                                               const UDamageType* DamageType, AController* InstigatedBy,
                                               AActor* DamageCauser)
{
	if (Health == 100.f)
	{
		return;
	}

	Cast<APlayerController>(GetController())->ClientStartCameraShake(HitCameraShakeClass);

	if (Health <= 0.0f && !HealthComponent->bDied)
	{
		Death();
	}
}

void AMultiShootGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bMoving = GetCharacterMovement()->Velocity.Size() > 0;

	if (bAimed)
	{
		const FVector StartLocation = FPSCameraSceneComponent->GetComponentLocation();

		const FVector CameraLocation = CameraComponent->GetComponentLocation();
		const FRotator CameraRotation = CameraComponent->GetComponentRotation();
		const FVector TargetLocation = CameraLocation + CameraRotation.Vector() * 3000.f;

		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);

		const FRotator TargetRotation = FRotator(0, LookAtRotation.Yaw - 90.f, LookAtRotation.Pitch * -1.f);

		FPSCameraSceneComponent->SetWorldRotation(TargetRotation);
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
	PlayerInputComponent->BindAction("MainWeapon", IE_Pressed, this, &AMultiShootGameCharacter::ToggleMainWeapon);
	PlayerInputComponent->BindAction("SecondWeapon", IE_Pressed, this, &AMultiShootGameCharacter::ToggleSecondWeapon);
	PlayerInputComponent->BindAction("ThirdWeapon", IE_Pressed, this, &AMultiShootGameCharacter::ToggleThirdWeapon);
	PlayerInputComponent->BindAction("ToggleWeaponUp", IE_Pressed, this, &AMultiShootGameCharacter::ToggleWeaponUp);
	PlayerInputComponent->BindAction("ToggleWeaponDown", IE_Pressed, this, &AMultiShootGameCharacter::ToggleWeaponDown);

	// Bind throw grenade
	PlayerInputComponent->BindAction("ThrowGrenade", IE_Pressed, this, &AMultiShootGameCharacter::BeginThrowGrenade);
	PlayerInputComponent->BindAction("ThrowGrenade", IE_Released, this, &AMultiShootGameCharacter::ThrowGrenade);

	// Bind knife attack
	PlayerInputComponent->BindAction("KnifeAttack", IE_Pressed, this, &AMultiShootGameCharacter::KnifeAttack);
}

void AMultiShootGameCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiShootGameCharacter, WeaponMode);
	DOREPLIFETIME(AMultiShootGameCharacter, bKnifeAttack);
	DOREPLIFETIME(AMultiShootGameCharacter, bBeginThrowGrenade);
	DOREPLIFETIME(AMultiShootGameCharacter, bThrowingGrenade);
	DOREPLIFETIME(AMultiShootGameCharacter, bSpawnGrenade);
}
