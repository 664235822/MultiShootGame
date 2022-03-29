// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiShootGameFPSCamera.h"
#include "Components/AudioComponent.h"
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

void AMultiShootGameFPSCamera::ShakeCamera()
{
	AMultiShootGameCharacter* MyOwner = Cast<AMultiShootGameCharacter>(GetOwner());
	if (MyOwner)
	{
		if (MyOwner->GetWeaponMode() == EWeaponMode::Weapon)
		{
			MyOwner->AddControllerYawInput(FMath::RandRange(-1 * CameraSpread, CameraSpread));
			MyOwner->AddControllerPitchInput(-1 * FMath::RandRange(0.f, CameraSpread));
		}

		APlayerController* PlayerController = Cast<APlayerController>(MyOwner->GetController());
		if (PlayerController)
		{
			switch (MyOwner->GetWeaponMode())
			{
			case EWeaponMode::Weapon:
				PlayerController->ClientStartCameraShake(FireCameraShake);
				break;
			case EWeaponMode::Sniper:
				PlayerController->ClientStartCameraShake(SniperCameraShake);
				break;
			case EWeaponMode::Shotgun:
				PlayerController->ClientStartCameraShake(ShotgunCameraShake);
				break;
			}
		}
	}
}

void AMultiShootGameFPSCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float TargetFOV = bWantToZoom ? ZoomedFOV : DefaultFOV;
	const float CurrentFOV = FMath::FInterpTo(CameraComponent->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);
	CameraComponent->SetFieldOfView(CurrentFOV);
}

void AMultiShootGameFPSCamera::ToggleWeapon(EWeaponMode WeaponMode)
{
	switch (WeaponMode)
	{
	case EWeaponMode::Weapon:
		if (WeaponMesh)
		{
			WeaponMeshComponent->SetSkeletalMesh(WeaponMesh);
			CameraComponent->SetRelativeTransform(FTransform(FQuat(FRotator(0, 90.f, 0)), FVector(0, 5.f, 15.f),
			                                                 FVector::OneVector));
			AudioComponent->SetSound(WeaponFireCue);
		}
		break;
	case EWeaponMode::Sniper:
		WeaponMeshComponent->SetSkeletalMesh(nullptr);
		AudioComponent->SetSound(SniperFireCue);
		break;
	case EWeaponMode::Shotgun:
		if (ShotgunMesh)
		{
			WeaponMeshComponent->SetSkeletalMesh(ShotgunMesh);
			CameraComponent->SetRelativeTransform(FTransform(FQuat(FRotator(0, 90.f, 0)), FVector(0, -15.f, 15.f),
			                                                 FVector::OneVector));
			AudioComponent->SetSound(ShotgunFireCue);
		}
		break;
	}
}

void AMultiShootGameFPSCamera::SetZoomed(bool WantToZoom)
{
	bWantToZoom = WantToZoom;
}

UCameraComponent* AMultiShootGameFPSCamera::GetCameraComponent() const
{
	return CameraComponent;
}
