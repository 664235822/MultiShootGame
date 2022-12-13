// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiShootGameWeapon.h"
#include "MultiShootGameProjectile.h"
#include "MultiShootGame/Character/MultiShootGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MultiShootGame/GameMode/MultiShootGameGameMode.h"
#include "MultiShootGame/SaveGame/ChooseWeaponSaveGame.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AMultiShootGameWeapon::AMultiShootGameWeapon()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bOnlyRelevantToOwner = true;

	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponmeshComponent"));
	WeaponMeshComponent->SetupAttachment(RootComponent);
	WeaponMeshComponent->SetCastHiddenShadow(true);
}

// Called when the game starts or when spawned
void AMultiShootGameWeapon::BeginPlay()
{
	Super::BeginPlay();

	const UChooseWeaponSaveGame* SaveGame = Cast<UChooseWeaponSaveGame>(
		UGameplayStatics::LoadGameFromSlot(TEXT("ChooseWeapon"), 0));
	if (SaveGame)
	{
		TArray<FWeaponInfo> WeaponInfoList;
		switch (CurrentWeaponMode)
		{
		case EWeaponMode::MainWeapon:
			WeaponInfoList = SaveGame->MainWeaponList;
			WeaponInfo = WeaponInfoList[SaveGame->MainWeaponIndex];
			break;
		case EWeaponMode::SecondWeapon:
			WeaponInfoList = SaveGame->SecondWeaponList;
			WeaponInfo = WeaponInfoList[SaveGame->SecondWeaponIndex];
			break;
		case EWeaponMode::ThirdWeapon:
			WeaponInfoList = SaveGame->ThirdWeaponList;
			WeaponInfo = WeaponInfoList[SaveGame->ThirdWeaponIndex];
			break;
		}
		WeaponMeshComponent->SetSkeletalMesh(WeaponInfo.WeaponMesh);
	}

	TimeBetweenShots = 60.0f / WeaponInfo.RateOfFire;
}

void AMultiShootGameWeapon::Fire()
{
	AMultiShootGameCharacter* MyOwner = Cast<AMultiShootGameCharacter>(GetOwner());

	if (BulletCheck(MyOwner))
	{
		return;
	}

	if (MyOwner)
	{
		const FVector EyeLocation = MyOwner->GetCameraComponent()->GetComponentLocation();
		const FRotator EyeRotation = MyOwner->GetCameraComponent()->GetComponentRotation();

		FVector ShotDirection = EyeRotation.Vector();

		const float HalfRad = FMath::DegreesToRadians(WeaponInfo.BulletSpread);
		ShotDirection = FMath::VRandCone(ShotDirection, HalfRad, HalfRad);

		const FVector TraceEnd = EyeLocation + (ShotDirection * 3000.f);

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
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponInfo.MuzzleEffect, MuzzleLocation);
				}
			}
			else
			{
				MyOwner->Fire_Server(WeaponInfo, MuzzleLocation, ShotTargetDirection);
			}
		}

		ShakeCamera();

		BulletFire(MyOwner);

		LastFireTime = GetWorld()->TimeSeconds;
	}
}

void AMultiShootGameWeapon::ShakeCamera()
{
	AMultiShootGameCharacter* MyOwner = Cast<AMultiShootGameCharacter>(GetOwner());
	if (MyOwner)
	{
		if (MyOwner->GetWeaponMode() == EWeaponMode::MainWeapon)
		{
			MyOwner->AddControllerYawInput(FMath::RandRange(-1 * WeaponInfo.CameraSpread, WeaponInfo.CameraSpread));
			MyOwner->AddControllerPitchInput(
				-1 * FMath::RandRange(-1 * WeaponInfo.CameraSpreadDown, WeaponInfo.CameraSpread));
		}

		APlayerController* PlayerController = Cast<APlayerController>(MyOwner->GetController());
		if (PlayerController)
		{
			PlayerController->ClientStartCameraShake(WeaponInfo.FireCameraShake);
		}
	}
}

void AMultiShootGameWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiShootGameWeapon, WeaponInfo);
}

void AMultiShootGameWeapon::StartFire()
{
	const float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMultiShootGameWeapon::Fire, TimeBetweenShots, true,
	                                FirstDelay);
}

void AMultiShootGameWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle);
}

void AMultiShootGameWeapon::FireOfDelay()
{
	if (LastFireTime == 0)
	{
		Fire();

		return;
	}

	const float CurrentFireTime = GetWorld()->GetTimeSeconds();

	if (CurrentFireTime - LastFireTime > WeaponInfo.DelayOfShotgun)
	{
		Fire();
	}
}

void AMultiShootGameWeapon::EnablePhysicsSimulate()
{
	WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	WeaponMeshComponent->SetCollisionProfileName("BlockAll");
	WeaponMeshComponent->SetSimulatePhysics(true);
}

void AMultiShootGameWeapon::ReloadShowMagazineClip(bool Enabled)
{
	if (Enabled)
	{
		WeaponMeshComponent->UnHideBoneByName(ClipBoneName);
	}
	else
	{
		WeaponMeshComponent->HideBoneByName(ClipBoneName, PBO_None);
		if (WeaponInfo.MagazineClipMesh)
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AMultiShootGameMagazineClip* CurrentMagazineClip = GetWorld()->SpawnActor<AMultiShootGameMagazineClip>(
				MagazineClipClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameters);
			CurrentMagazineClip->SetOwner(this);
			CurrentMagazineClip->AttachToComponent(WeaponMeshComponent,
			                                       FAttachmentTransformRules::SnapToTargetIncludingScale,
			                                       FName("Magazine"));

			CurrentMagazineClip->ThrowMagazineClip(WeaponInfo.MagazineClipMesh);
		}
	}
}

bool AMultiShootGameWeapon::BulletCheck(AMultiShootGameCharacter* MyOwner)
{
	if (WeaponInfo.BulletNumber == 0 && WeaponInfo.MaxBulletNumber == 0)
	{
		return true;
	}

	return false;
}

void AMultiShootGameWeapon::BulletFire(AMultiShootGameCharacter* MyOwner)
{
	if (WeaponInfo.BulletNumber > 0)
	{
		WeaponInfo.BulletNumber--;
	}
	else
	{
		MyOwner->BeginReload();
	}
}

void AMultiShootGameWeapon::BulletReload()
{
	if (WeaponInfo.MaxBulletNumber > WeaponInfo.FillUpBulletNumber)
	{
		if (WeaponInfo.BulletNumber < WeaponInfo.FillUpBulletNumber)
		{
			const int TempNumber = WeaponInfo.FillUpBulletNumber - WeaponInfo.BulletNumber;
			WeaponInfo.BulletNumber = WeaponInfo.FillUpBulletNumber;
			WeaponInfo.MaxBulletNumber -= TempNumber;
		}
		else
		{
			WeaponInfo.BulletNumber += WeaponInfo.FillUpBulletNumber;
			WeaponInfo.MaxBulletNumber -= WeaponInfo.FillUpBulletNumber;
		}
	}
	else
	{
		if (WeaponInfo.BulletNumber + WeaponInfo.MaxBulletNumber > WeaponInfo.FillUpBulletNumber)
		{
			const int TempNumber = WeaponInfo.FillUpBulletNumber - WeaponInfo.BulletNumber;
			WeaponInfo.BulletNumber = WeaponInfo.FillUpBulletNumber;
			WeaponInfo.MaxBulletNumber -= TempNumber;;
		}
		else
		{
			WeaponInfo.BulletNumber += WeaponInfo.MaxBulletNumber;
			WeaponInfo.MaxBulletNumber = 0;
		}
	}
}

void AMultiShootGameWeapon::FillUpBullet()
{
	WeaponInfo.BulletNumber = WeaponInfo.FillUpBulletNumber;
	WeaponInfo.MaxBulletNumber = WeaponInfo.FillUpMaxBulletNumber;
}
