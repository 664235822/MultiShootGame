// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiShootGameCharacter.h"
#include "MultiShootGameEnemyCharacter.h"
#include "MultiShootGame/MultiShootGame.h"
#include "MultiShootGame/Weapon/MultiShootGameProjectile.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Microsoft/AllowMicrosoftPlatformTypes.h"
#include "MultiShootGame/GameMode/MultiShootGameGameMode.h"
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
	GetCharacterMovement()->SetIsReplicated(true);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->SetIsReplicated(true);
	if (GetLocalRole() == ROLE_Authority)
	{
		HealthComponent->OnHealthChanged.AddDynamic(this, &AMultiShootGameCharacter::OnHealthChanged);
		HealthComponent->OnHeadShot.AddDynamic(this, &AMultiShootGameCharacter::HeadShot);
	}

	HitEffectComponent = CreateDefaultSubobject<UHitEffectComponent>(TEXT("HitEfectComponent"));
}

void AMultiShootGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentGameMode = UGameplayStatics::GetGameMode(GetWorld());
	if (Cast<AMultiShootGameGameMode>(CurrentGameMode))
	{
		if (IsLocallyControlled())
		{
			CurrentGameUserWidget = CreateWidget(GetWorld(), GameUserWidgetClass);
		}
	}
	else
	{
		if (IsLocallyControlled())
		{
			CurrentGameUserWidget = CreateWidget(GetWorld(), ServerGameUserWidgetClass);
		}
	}

	if (IsLocallyControlled())
	{
		CurrentGameUserWidget->AddToViewport();
	}


	APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	PlayerCameraManager->ViewPitchMax = CameraPitchClamp;
	PlayerCameraManager->ViewPitchMin = -1 * CameraPitchClamp;

	GrenadeCount = MaxGrenadeCount;

	if (IsLocallyControlled())
	{
		CurrentSniperUserWidget = CreateWidget(GetWorld(), SniperUserWidgetClass);
		CurrentSniperUserWidget->AddToViewport();
		CurrentSniperUserWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.Instigator = GetInstigator();
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
		CurrentMainWeapon->AttachToComponent(MainWeaponSceneComponent,
		                                     FAttachmentTransformRules::SnapToTargetIncludingScale);
	}

	if (CurrentSecondWeapon)
	{
		CurrentSecondWeapon->AttachToComponent(SecondWeaponSceneComponent,
		                                       FAttachmentTransformRules::SnapToTargetIncludingScale);
	}

	if (CurrentThirdWeapon)
	{
		CurrentThirdWeapon->AttachToComponent(ThirdWeaponSceneComponent,
		                                      FAttachmentTransformRules::SnapToTargetIncludingScale);
	}

	if (CurrentFPSCamera)
	{
		CurrentFPSCamera->AttachToComponent(FPSCameraSceneComponent,
		                                    FAttachmentTransformRules::SnapToTargetIncludingScale);
		CurrentFPSCamera->SetActorHiddenInGame(true);
		CurrentFPSCamera->SetWeaponInfo(CurrentMainWeapon);
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

	if (CurrentGrenade)
	{
		CurrentGrenade->Destroy();
	}

	if (IsLocallyControlled())
	{
		CurrentGameUserWidget->RemoveFromParent();
		CurrentSniperUserWidget->RemoveFromParent();
	}

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
	HandleWalkSpeed(true);
}

void AMultiShootGameCharacter::EndFastRun()
{
	HandleWalkSpeed(false);
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

	if (WeaponMode == EWeaponMode::SecondWeapon && CurrentSecondWeapon->WeaponInfo.AimTexture)
	{
		CurrentSniperUserWidget->SetVisibility(ESlateVisibility::Visible);
	}

	if (WeaponMode == EWeaponMode::MainWeapon && bFired)
	{
		CurrentFPSCamera->StartFire();
	}

	if (IsLocallyControlled())
	{
		CurrentMainWeapon->StopFire();
	}

	PlayAnimMontage_Server(AimedAnimMontage);
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

	if (CurrentSecondWeapon->WeaponInfo.AimTexture)
	{
		CurrentSniperUserWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (WeaponMode == EWeaponMode::MainWeapon && bFired)
	{
		CurrentMainWeapon->StartFire();
	}

	CurrentFPSCamera->StopFire();

	StopAnimMontage_Server(AimedAnimMontage);
}

void AMultiShootGameCharacter::Fire_Server_Implementation(FWeaponInfo WeaponInfo, FVector MuzzleLocation,
                                                          FRotator ShotTargetDirection, FName MuzzleSocketName)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.Instigator = GetInstigator();
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AMultiShootGameProjectileBase* CurrentProjectile = GetWorld()->SpawnActor<AMultiShootGameProjectileBase>(
		WeaponInfo.ProjectileClass, MuzzleLocation, ShotTargetDirection, SpawnParameters);
	if (CurrentProjectile)
	{
		CurrentProjectile->ProjectileInitialize(WeaponInfo.BaseDamage);
	}

	Fire_Multicast(WeaponInfo, MuzzleSocketName);
}

void AMultiShootGameCharacter::Fire_Multicast_Implementation(FWeaponInfo WeaponInfo, FName MuzzleSocketName)
{
	USkeletalMeshComponent* WeaponMeshComponent = nullptr;

	switch (WeaponMode)
	{
	case EWeaponMode::MainWeapon:
		WeaponMeshComponent = CurrentMainWeapon->GetWeaponMeshComponent();
		break;
	case EWeaponMode::SecondWeapon:
		WeaponMeshComponent = CurrentSecondWeapon->GetWeaponMeshComponent();
		break;
	case EWeaponMode::ThirdWeapon:
		WeaponMeshComponent = CurrentThirdWeapon->GetWeaponMeshComponent();
		break;
	}

	if (WeaponInfo.FireSoundCue)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponInfo.FireSoundCue,
		                                      WeaponMeshComponent->GetSocketLocation(MuzzleSocketName));
	}

	if (WeaponInfo.MuzzleEffect)
	{
		if (bAimed && !IsLocallyControlled() || !bAimed)
		{
			UGameplayStatics::SpawnEmitterAttached(WeaponInfo.MuzzleEffect, WeaponMeshComponent, MuzzleSocketName);
		}
	}
}

void AMultiShootGameCharacter::ThrowBulletShell_Server_Implementation(
	TSubclassOf<AMultiShootGameBulletShell> BulletShellClass, FVector BulletShellLocation,FRotator BulletShellRotation)
{
	if (BulletShellClass)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.Instigator = GetInstigator();
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		AMultiShootGameBulletShell* BulletShell = GetWorld()->SpawnActor<AMultiShootGameBulletShell>(
			BulletShellClass, BulletShellLocation, BulletShellRotation, SpawnParameters);
		BulletShell->ThrowBulletShell_Server();
	}
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

void AMultiShootGameCharacter::EndThrowGrenade()
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

		ThrowGrenadeOut_Server(LookAtRotation, bFastRun || GetCharacterMovement()->IsFalling());

		SetGrenadeCount_Server(FMath::Clamp(GrenadeCount - 1, 0, MaxGrenadeCount));
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
		SpawnParameters.Owner = this;
		SpawnParameters.Instigator = GetInstigator();
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		CurrentGrenade = GetWorld()->SpawnActor<AMultiShootGameGrenade>(GrenadeClass, FVector::ZeroVector,
		                                                                FRotator::ZeroRotator, SpawnParameters);

		if (CurrentGrenade)
		{
			CurrentGrenade->BaseDamage = GrenadeDamage;
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
		KnifeSkeletalMeshComponent->SetVisibility(true);

		PutBackWeapon_Server();
	}
}

void AMultiShootGameCharacter::EndKnifeAttack_Server_Implementation()
{
	if (bKnifeAttack)
	{
		bToggleWeapon = true;

		KnifeSkeletalMeshComponent->SetVisibility(false);

		PlayAnimMontage_Server(WeaponOutAnimMontage);
	}
}

void AMultiShootGameCharacter::KnifeHit_Server_Implementation()
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
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(this);
		if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), HitLocation, HitLocation, 50.f, TraceType_WeaponTrace,
		                                            false, IgnoreActors, EDrawDebugTrace::None, HitResult, true))
		{
			const EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());

			UGameplayStatics::ApplyDamage(HitResult.GetActor(), KnifeDamage, GetInstigatorController(), this,
			                              DamageTypeClass);

			HitEffectComponent->PlayHitEffect(SurfaceType, HitLocation, HitRotation);
		}
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

	HandleWalkSpeed(bFastRun);

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

	HandleWalkSpeed(bFastRun);

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

	HandleWalkSpeed(bFastRun);

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

void AMultiShootGameCharacter::FillUpWeaponBullet()
{
	CurrentMainWeapon->FillUpBullet();
	CurrentSecondWeapon->FillUpBullet();
	CurrentThirdWeapon->FillUpBullet();
	CurrentFPSCamera->FillUpBullet();
	GrenadeCount = MaxGrenadeCount;
}

void AMultiShootGameCharacter::HeadShot(AActor* DamageCauser)
{
	AMultiShootGameCharacter* Character = Cast<AMultiShootGameCharacter>(DamageCauser);
	if (Character)
	{
		Character->OnHeadshot_Server();
	}
}

void AMultiShootGameCharacter::Reborn_Server_Implementation()
{
	TArray<AActor*> OutActorArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlayerStartClass, OutActorArray);
	const AActor* OutActor = OutActorArray[UKismetMathLibrary::RandomInteger(OutActorArray.Max())];
	const FTransform Transform = OutActor->GetActorTransform();

	AMultiShootGameCharacter* Character = GetWorld()->SpawnActor<AMultiShootGameCharacter>(CharacterClass, Transform);
	GetController()->Possess(Character);

	if (Character)
	{
		Character->Score = Score;
		Character->KillCount = KillCount;
		Character->DeathCount = DeathCount;
	}

	Destroy(true);
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

void AMultiShootGameCharacter::HandleWalkSpeed(bool FastRun)
{
	float Speed = 300.f;
	if (FastRun)
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

	SetFastRun_Server(FastRun);
	GetCharacterMovement()->MaxWalkSpeed = Speed;
	SetWalkSpeed_Server(Speed);
}

void AMultiShootGameCharacter::SetFastRun_Server_Implementation(bool Value)
{
	bFastRun = Value;
}

void AMultiShootGameCharacter::CheckShowSight(float DeltaSeconds)
{
	if (bShowSight)
	{
		if (CurrentShowSight < ShowSightDelay)
		{
			CurrentShowSight += DeltaSeconds;
		}
		else
		{
			bShowSight = false;
			CurrentShowSight = 0.f;
		}
	}
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

void AMultiShootGameCharacter::SetGrenadeCount_Server_Implementation(int Value)
{
	GrenadeCount = Value;
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

void AMultiShootGameCharacter::StopAnimMontage_Server_Implementation(UAnimMontage* AnimMontage)
{
	StopAnimMontage_Multicast(AnimMontage);
}

void AMultiShootGameCharacter::StopAnimMontage_Multicast_Implementation(UAnimMontage* AnimMontage)
{
	StopAnimMontage(AnimMontage);
}

void AMultiShootGameCharacter::Death_Server_Implementation()
{
	HealthComponent->bDied = true;

	Death_Multicast();
}

void AMultiShootGameCharacter::Death_Multicast_Implementation()
{
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
		OnDeath();

		AMultiShootGameCharacter* Character = Cast<AMultiShootGameCharacter>(DamageCauser);
		if (Character)
		{
			Character->OnEnemyKilled_Server();
		}
		Death_Server();
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

	CheckShowSight(DeltaTime);
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
	DOREPLIFETIME(AMultiShootGameCharacter, bFastRun);
	DOREPLIFETIME(AMultiShootGameCharacter, bKnifeAttack);
	DOREPLIFETIME(AMultiShootGameCharacter, bBeginThrowGrenade);
	DOREPLIFETIME(AMultiShootGameCharacter, bThrowingGrenade);
	DOREPLIFETIME(AMultiShootGameCharacter, bSpawnGrenade);
	DOREPLIFETIME(AMultiShootGameCharacter, GrenadeCount);
	DOREPLIFETIME(AMultiShootGameCharacter, bDetectingClimb);
	DOREPLIFETIME(AMultiShootGameCharacter, bShowSight);
	DOREPLIFETIME(AMultiShootGameCharacter, Score);
	DOREPLIFETIME(AMultiShootGameCharacter, KillCount);
	DOREPLIFETIME(AMultiShootGameCharacter, DeathCount);
}

void AMultiShootGameCharacter::OnEnemyKilled_Server_Implementation()
{
	Score += 50;
	KillCount++;
	bShowSight = true;
	CurrentShowSight = 0.f;
}

void AMultiShootGameCharacter::OnHeadshot_Server_Implementation()
{
	Score += 25;
}

void AMultiShootGameCharacter::OnDeath()
{
	DeathCount++;
}
