// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiShootGameFPSCamera.h"

#include "MultiShootGameGameMode.h"

AMultiShootGameFPSCamera::AMultiShootGameFPSCamera()
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(WeaponMeshComponent);
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
		}
		break;
	case EWeaponMode::Sniper:
		WeaponMeshComponent->SetSkeletalMesh(nullptr);
		break;
	case EWeaponMode::Shotgun:
		if (ShotgunMesh)
		{
			WeaponMeshComponent->SetSkeletalMesh(ShotgunMesh);
			CameraComponent->SetRelativeTransform(FTransform(FQuat(FRotator(0, 90.f, 0)), FVector(0, -15.f, 15.f),
			                                                 FVector::OneVector));
		}
		break;
	}
}
