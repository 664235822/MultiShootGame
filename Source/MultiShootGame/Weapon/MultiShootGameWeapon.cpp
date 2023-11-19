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
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;
	bOnlyRelevantToOwner = true;

	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponmeshComponent"));
	WeaponMeshComponent->SetupAttachment(RootComponent);
	WeaponMeshComponent->SetCastHiddenShadow(true);
	WeaponMeshComponent->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void AMultiShootGameWeapon::BeginPlay()
{
	Super::BeginPlay();

	AMultiShootGameCharacter* Character = Cast<AMultiShootGameCharacter>(GetOwner());
	if (Character->IsLocallyControlled())
	{
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
		}

		bInitializeReady = true;
	}

	TimeBetweenShots = 60.0f / WeaponInfo.RateOfFire;
}

void AMultiShootGameWeapon::Fire()
{
	AMultiShootGameCharacter* MyOwner = Cast<AMultiShootGameCharacter>(GetOwner());

	if (BulletCheck(MyOwner))
	{
		StopFireCurve();
		
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

void AMultiShootGameWeapon::ShakeCamera()
{
	AMultiShootGameCharacter* MyOwner = Cast<AMultiShootGameCharacter>(GetOwner());
	if (MyOwner)
	{
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

	//DOREPLIFETIME(AMultiShootGameWeapon, WeaponInfo);
}

void AMultiShootGameWeapon::StartFire()
{
	const float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);

	AMultiShootGameCharacter* MyOwner = Cast<AMultiShootGameCharacter>(GetOwner());

	if (BulletCheck(MyOwner))
	{
		StopFireCurve();

		return;
	}
	
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMultiShootGameWeapon::Fire, TimeBetweenShots, true,
	                                FirstDelay);
	StartFireCurve();
}

void AMultiShootGameWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle);

	StopFireCurve();
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
	WeaponMeshComponent->SetCollisionProfileName(FName("Ragdoll"));
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
			if(!Cast<AMultiShootGameCharacter>(GetOwner())->GetToggleViewed())
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
