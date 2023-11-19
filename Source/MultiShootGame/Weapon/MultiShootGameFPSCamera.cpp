// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiShootGameFPSCamera.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MultiShootGame/Character/MultiShootGameCharacter.h"
#include "MultiShootGame/GameMode/MultiShootGameGameMode.h"

AMultiShootGameFPSCamera::AMultiShootGameFPSCamera()
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	RootComponent = CameraComponent;

	ArmsMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmsMeshComponent"));
	ArmsMeshComponent->SetupAttachment(CameraComponent);
	ArmsMeshComponent->SetCastHiddenShadow(true);
	ArmsMeshComponent->SetIsReplicated(true);

	WeaponMeshComponent->SetupAttachment(ArmsMeshComponent, WeaponSocketName);
}

void AMultiShootGameFPSCamera::BeginPlay()
{
	Super::BeginPlay();

	DefaultFOV = CameraComponent->FieldOfView;
	bInitializeReady = true;

	CurrentSniperUserWidget = CreateWidget(GetWorld(), SniperUserWidgetClass);
	CurrentSniperUserWidget->AddToViewport();
	CurrentSniperUserWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AMultiShootGameFPSCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AMultiShootGameCharacter* Character = Cast<AMultiShootGameCharacter>(GetOwner());
	if (Character)
	{
		Speed = Character->GetCharacterMovement()->Velocity.Size();
		bFired = Character->GetFired();
		bAimed = Character->GetAimed();
		bReloading = Character->GetReloading();
		bToggleWeapon = Character->GetToggleWeapon();
	}

	const float TargetFOV = WeaponInfo.SniperAim && bAimed ? ZoomedFOV : DefaultFOV;
	const float CurrentFOV = FMath::FInterpTo(CameraComponent->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);
	CameraComponent->SetFieldOfView(CurrentFOV);
}

void AMultiShootGameFPSCamera::Fire()
{
	AMultiShootGameCharacter* MyOwner = Cast<AMultiShootGameCharacter>(GetOwner());

	if (MyOwner)
	{
		FVector EyeLocation = MyOwner->GetCurrentFPSCamera()->GetCameraComponent()->GetComponentLocation();
		FRotator EyeRotation = MyOwner->GetCurrentFPSCamera()->GetCameraComponent()->GetComponentRotation();

		FVector ShotDirection = EyeRotation.Vector();

		const float HalfRad = FMath::DegreesToRadians(WeaponInfo.BulletSpread);
		ShotDirection = FMath::VRandCone(ShotDirection, HalfRad, HalfRad);

		const FVector TraceEnd = EyeLocation + (ShotDirection * 3000.f);

		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(EyeLocation, TraceEnd);
		const FRotator TargetRotation = FRotator(LookAtRotation.Pitch, LookAtRotation.Yaw, 0);

		MyOwner->GetFPSCameraSceneComponent()->SetWorldRotation(TargetRotation);

		if (WeaponInfo.ProjectileClass)
		{
			const FVector MuzzleLocation = WeaponMeshComponent->GetSocketLocation(MuzzleSocketName);

			const FRotator ShotTargetDirection = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, TraceEnd);

			AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
			if (Cast<AMultiShootGameGameMode>(GameMode))
			{
				FActorSpawnParameters SpawnParameters;
				SpawnParameters.Owner = GetOwner();
				SpawnParameters.Instigator = GetInstigator();
				SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				AMultiShootGameProjectileBase* CurrentProjectile = GetWorld()->SpawnActor<
					AMultiShootGameProjectileBase>(WeaponInfo.ProjectileClass, MuzzleLocation, ShotTargetDirection,
					                               SpawnParameters);
				CurrentProjectile->ProjectileInitialize(WeaponInfo.BaseDamage);

				if (WeaponInfo.FireSoundCue)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponInfo.FireSoundCue, MuzzleLocation);
				}

				if (WeaponInfo.MuzzleEffect)
				{
					UGameplayStatics::SpawnEmitterAttached(WeaponInfo.MuzzleEffect, WeaponMeshComponent,
					                                       MuzzleSocketName);
				}

				if (BulletShellClass)
				{
					const FVector BulletShellLocation = WeaponMeshComponent->GetSocketLocation(BulletShellName);
					const FRotator BulletShellRotation = WeaponMeshComponent->GetComponentRotation();

					AMultiShootGameBulletShell* BulletShell = GetWorld()->SpawnActor<AMultiShootGameBulletShell>(
						BulletShellClass, BulletShellLocation, BulletShellRotation, SpawnParameters);
					BulletShell->ThrowBulletShell_Server();
				}
			}
			else
			{
				MyOwner->Fire_Server(WeaponInfo, MuzzleLocation, ShotTargetDirection, MuzzleSocketName);

				if (WeaponInfo.MuzzleEffect)
				{
					UGameplayStatics::SpawnEmitterAttached(WeaponInfo.MuzzleEffect, WeaponMeshComponent,
					                                       MuzzleSocketName);
				}

				if (BulletShellClass)
				{
					const FVector BulletShellLocation = WeaponMeshComponent->GetSocketLocation(BulletShellName);
					const FRotator BulletShellRotation = WeaponMeshComponent->GetComponentRotation();

					MyOwner->ThrowBulletShell_Server(BulletShellClass, BulletShellLocation, BulletShellRotation);
				}
			}
		}

		ShakeCamera();

		BulletFire(MyOwner);

		LastFireTime = GetWorld()->TimeSeconds;
	}
}

bool AMultiShootGameFPSCamera::BulletCheck(AMultiShootGameCharacter* MyOwner)
{
	switch (MyOwner->GetWeaponMode())
	{
	case EWeaponMode::MainWeapon:
		if (MyOwner->GetCurrentMainWeapon()->WeaponInfo.BulletNumber == 0 &&
			MyOwner->GetCurrentMainWeapon()->WeaponInfo.MaxBulletNumber == 0)
		{
			return true;
		}
	case EWeaponMode::SecondWeapon:
		if (MyOwner->GetCurrentSecondWeapon()->WeaponInfo.BulletNumber == 0 &&
			MyOwner->GetCurrentSecondWeapon()->WeaponInfo.MaxBulletNumber == 0)
		{
			return true;
		}
	case EWeaponMode::ThirdWeapon:
		if (MyOwner->GetCurrentThirdWeapon()->WeaponInfo.BulletNumber == 0 &&
			MyOwner->GetCurrentThirdWeapon()->WeaponInfo.MaxBulletNumber == 0)
		{
			return true;
		}
	}
	return false;
}

void AMultiShootGameFPSCamera::BulletFire(AMultiShootGameCharacter* MyOwner)
{
	switch (MyOwner->GetWeaponMode())
	{
	case EWeaponMode::MainWeapon:
		if (MyOwner->GetCurrentMainWeapon()->WeaponInfo.BulletNumber > 0)
		{
			MyOwner->GetCurrentMainWeapon()->WeaponInfo.BulletNumber--;
		}
		else
		{
			MyOwner->BeginReload();
		}
		break;
	case EWeaponMode::SecondWeapon:
		if (MyOwner->GetCurrentSecondWeapon()->WeaponInfo.BulletNumber > 0)
		{
			MyOwner->GetCurrentSecondWeapon()->WeaponInfo.BulletNumber--;
		}
		else
		{
			MyOwner->BeginReload();
		}
		break;
	case EWeaponMode::ThirdWeapon:
		if (MyOwner->GetCurrentThirdWeapon()->WeaponInfo.BulletNumber > 0)
		{
			MyOwner->GetCurrentThirdWeapon()->WeaponInfo.BulletNumber--;
		}
		else
		{
			MyOwner->BeginReload();
		}
	}
}

void AMultiShootGameFPSCamera::ReloadShowMagazineClip(bool Enabled)
{
	if (Enabled)
	{
		WeaponMeshComponent->UnHideBoneByName(ClipBoneName);
		if (CurrentMagazineClip)
		{
			CurrentMagazineClip->Destroy();
		}
	}
	else
	{
		WeaponMeshComponent->HideBoneByName(ClipBoneName, PBO_None);
		if (WeaponInfo.MagazineClipMesh)
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			CurrentMagazineClip = GetWorld()->SpawnActor<AMultiShootGameMagazineClip>(
				MagazineClipClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameters);
			CurrentMagazineClip->SetOwner(this);
			CurrentMagazineClip->AttachToComponent(ArmsMeshComponent,
			                                       FAttachmentTransformRules::SnapToTargetIncludingScale,
			                                       FName("Magazine"));
		}
	}
}

void AMultiShootGameFPSCamera::InspectBegin()
{
	bInspect = true;
}

void AMultiShootGameFPSCamera::InspectEnd()
{
	bInspect = false;
}

void AMultiShootGameFPSCamera::SetWeaponMesh()
{
	WeaponMeshComponent->SetSkeletalMesh(WeaponInfo.WeaponMesh);
}

void AMultiShootGameFPSCamera::SetWeaponInfo(FWeaponInfo Info)
{
	WeaponInfo = Info;
	if (!Cast<AMultiShootGameCharacter>(GetOwner())->GetToggleViewed() && !bAimed)
	{
		WeaponMeshComponent->SetSkeletalMesh(Info.WeaponMesh);
	}
	ArmsMeshComponent->SetRelativeTransform(FTransform(FQuat(FRotator::ZeroRotator),
	                                                   Info.AimVector + FVector(0, 0, -165),
	                                                   FVector::OneVector));
}

void AMultiShootGameFPSCamera::BeginAim(EWeaponMode WeaponMode)
{
	if (WeaponInfo.SniperAim && WeaponMode == EWeaponMode::SecondWeapon)
	{
		this->SetActorHiddenInGame(true);
		CurrentSniperUserWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		this->SetActorHiddenInGame(false);
	}
}

void AMultiShootGameFPSCamera::EndAim()
{
	if (WeaponInfo.SniperAim)
	{
		this->SetActorHiddenInGame(false);
		CurrentSniperUserWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
