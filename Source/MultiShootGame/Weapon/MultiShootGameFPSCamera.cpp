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

void AMultiShootGameFPSCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float TargetFOV = WeaponInfo.AimTexture ? ZoomedFOV : DefaultFOV;
	const float CurrentFOV = FMath::FInterpTo(CameraComponent->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);
	CameraComponent->SetFieldOfView(CurrentFOV);
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
