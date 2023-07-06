// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiShootGameFPSCamera.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MultiShootGame/Character/MultiShootGameCharacter.h"
#include "MultiShootGame/GameMode/MultiShootGameGameMode.h"

AMultiShootGameFPSCamera::AMultiShootGameFPSCamera()
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(WeaponMeshComponent);

	ArmsMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmsMeshComponent"));
	ArmsMeshComponent->SetupAttachment(WeaponMeshComponent);
}

void AMultiShootGameFPSCamera::BeginPlay()
{
	Super::BeginPlay();

	DefaultFOV = CameraComponent->FieldOfView;
	bInitializeReady = true;
}

void AMultiShootGameFPSCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float TargetFOV = WeaponInfo.AimTexture ? ZoomedFOV : DefaultFOV;
	const float CurrentFOV = FMath::FInterpTo(CameraComponent->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);
	CameraComponent->SetFieldOfView(CurrentFOV);
}

void AMultiShootGameFPSCamera::Fire()
{
	AMultiShootGameCharacter* MyOwner = Cast<AMultiShootGameCharacter>(GetOwner());

	if (BulletCheck(MyOwner))
	{
		return;
	}

	if (MyOwner)
	{
		FVector EyeLocation = MyOwner->GetCurrentFPSCamera()->GetCameraComponent()->GetComponentLocation();
		FRotator EyeRotation = MyOwner->GetCurrentFPSCamera()->GetCameraComponent()->GetComponentRotation();

		FVector ShotDirection = EyeRotation.Vector();

		const float HalfRad = FMath::DegreesToRadians(WeaponInfo.BulletSpread);
		ShotDirection = FMath::VRandCone(ShotDirection, HalfRad, HalfRad);

		const FVector TraceEnd = EyeLocation + (ShotDirection * 3000.f);

		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(EyeLocation, TraceEnd);
		const FRotator TargetRotation = FRotator(0, LookAtRotation.Yaw - 90.f, LookAtRotation.Pitch * -1.f);

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

void AMultiShootGameFPSCamera::SetWeaponInfo(FWeaponInfo Info)
{
	WeaponMeshComponent->SetSkeletalMesh(!Info.AimTexture
		                                     ? Info.WeaponMesh
		                                     : nullptr);
	CameraComponent->SetRelativeTransform(FTransform(FQuat(FRotator(0, 90.f, 0)),
	                                                 Info.AimVector,
	                                                 FVector::OneVector));
}
