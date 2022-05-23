// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiShootGameFPSCamera.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MultiShootGame/Character/MultiShootGameCharacter.h"

AMultiShootGameFPSCamera::AMultiShootGameFPSCamera()
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(WeaponMeshComponent);
}

void AMultiShootGameFPSCamera::BeginPlay()
{
	Super::BeginPlay();

	DefaultFOV = CameraComponent->FieldOfView;
}

void AMultiShootGameFPSCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float TargetFOV = WeaponInfo.AimTexture ? ZoomedFOV : DefaultFOV;
	const float CurrentFOV = FMath::FInterpTo(CameraComponent->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);
	CameraComponent->SetFieldOfView(CurrentFOV);
}

bool AMultiShootGameFPSCamera::BulletCheck(AMultiShootGameCharacter* MyOwner)
{
	switch (MyOwner->GetWeaponMode())
	{
	case EWeaponMode::MainWeapon:
		if (MyOwner->CurrentMainWeapon->WeaponInfo.BulletNumber == 0 &&
			MyOwner->CurrentMainWeapon->WeaponInfo.MaxBulletNumber == 0)
		{
			return true;
		}
	case EWeaponMode::SecondWeapon:
		if (MyOwner->CurrentSecondWeapon->WeaponInfo.BulletNumber == 0 &&
			MyOwner->CurrentSecondWeapon->WeaponInfo.MaxBulletNumber == 0)
		{
			return true;
		}
	case EWeaponMode::ThirdWeapon:
		if (MyOwner->CurrentThirdWeapon->WeaponInfo.BulletNumber == 0 &&
			MyOwner->CurrentThirdWeapon->WeaponInfo.MaxBulletNumber == 0)
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
		if (MyOwner->CurrentMainWeapon->WeaponInfo.BulletNumber > 0)
		{
			MyOwner->CurrentMainWeapon->WeaponInfo.BulletNumber--;
		}
		else
		{
			MyOwner->BeginReload();
		}
		break;
	case EWeaponMode::SecondWeapon:
		if (MyOwner->CurrentSecondWeapon->WeaponInfo.BulletNumber > 0)
		{
			MyOwner->CurrentSecondWeapon->WeaponInfo.BulletNumber--;
		}
		else
		{
			MyOwner->BeginReload();
		}
		break;
	case EWeaponMode::ThirdWeapon:
		if (MyOwner->CurrentThirdWeapon->WeaponInfo.BulletNumber > 0)
		{
			MyOwner->CurrentThirdWeapon->WeaponInfo.BulletNumber--;
		}
		else
		{
			MyOwner->BeginReload();
		}
	}
}

UCameraComponent* AMultiShootGameFPSCamera::GetCameraComponent() const
{
	return CameraComponent;
}

void AMultiShootGameFPSCamera::SetWeaponInfo(const AMultiShootGameWeapon* Weapon)
{
	WeaponInfo = Weapon->WeaponInfo;
	WeaponMeshComponent->SetSkeletalMesh(!Weapon->WeaponInfo.AimTexture
		                                     ? Weapon->GetWeaponMeshComponent()->SkeletalMesh
		                                     : nullptr);

	CameraComponent->SetRelativeTransform(FTransform(FQuat(FRotator(0, 90.f, 0)),
	                                                 Weapon->WeaponInfo.AimVector,
	                                                 FVector::OneVector));
	AudioComponent->Stop();
	AudioComponent->SetSound(Weapon->GetAudioComponent()->Sound);
}
