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
	
	const float TargetFOV = AimTexture ? ZoomedFOV : DefaultFOV;
	const float CurrentFOV = FMath::FInterpTo(CameraComponent->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);
	CameraComponent->SetFieldOfView(CurrentFOV);
}

UCameraComponent* AMultiShootGameFPSCamera::GetCameraComponent() const
{
	return CameraComponent;
}