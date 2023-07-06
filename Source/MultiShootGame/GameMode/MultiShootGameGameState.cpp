// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiShootGameGameState.h"
#include "MultiShootGamePlayerState.h"
#include "MultiShootGame/Character/MultiShootGameCharacter.h"

void AMultiShootGameGameState::HandleCharacterWeaponMesh()
{
	AMultiShootGamePlayerState* PlayerState = Cast<AMultiShootGamePlayerState>(PlayerArray[0]);
	const AMultiShootGameCharacter* Character = Cast<AMultiShootGameCharacter>(PlayerState->GetPawn());
	
	Character->GetCurrentMainWeapon()->GetWeaponMeshComponent()->SetSkeletalMesh(PlayerState->GetMainWeaponMesh());
	Character->GetCurrentSecondWeapon()->GetWeaponMeshComponent()->SetSkeletalMesh(PlayerState->GetSecondWeaponMesh());
	Character->GetCurrentThirdWeapon()->GetWeaponMeshComponent()->SetSkeletalMesh(PlayerState->GetThirdWeaponMesh());
	FWeaponInfo WeaponInfo;
	switch (Character->GetWeaponMode())
	{
	case EWeaponMode::MainWeapon:
		WeaponInfo = Character->GetCurrentMainWeapon()->WeaponInfo;
		break;
	case EWeaponMode::SecondWeapon:
		WeaponInfo = Character->GetCurrentSecondWeapon()->WeaponInfo;
		break;
	case EWeaponMode::ThirdWeapon:
		WeaponInfo = Character->GetCurrentThirdWeapon()->WeaponInfo;
		break;
	}
	Character->GetCurrentFPSCamera()->SetWeaponInfo(WeaponInfo);
}

void AMultiShootGameGameState::SetWaveState(EWaveState NewWaveState)
{
	const EWaveState OldState = WaveState;
	WaveState = NewWaveState;
}
