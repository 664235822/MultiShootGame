// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiShootGameServerGameState.h"
#include "MultiShootGamePlayerState.h"
#include "MultiShootGame/Character/MultiShootGameCharacter.h"

void AMultiShootGameServerGameState::HandleCharacterWeaponMesh_Server_Implementation()
{
	HandleCharacterWeaponMesh_Multicast();
}

void AMultiShootGameServerGameState::HandleCharacterWeaponMesh_Multicast_Implementation()
{
	for (APlayerState* PlayerState : PlayerArray)
	{
		AMultiShootGamePlayerState* CharacterPlayerState = Cast<AMultiShootGamePlayerState>(PlayerState);
		AMultiShootGameCharacter* Character = Cast<AMultiShootGameCharacter>(CharacterPlayerState->GetOwner());
		Character->GetCurrentMainWeapon()->GetWeaponMeshComponent()->SetSkeletalMesh(
			CharacterPlayerState->GetMainWeaponInfo().WeaponMesh);
		Character->GetCurrentSecondWeapon()->GetWeaponMeshComponent()->SetSkeletalMesh(
			CharacterPlayerState->GetSecondWeaponInfo().WeaponMesh);
		Character->GetCurrentThirdWeapon()->GetWeaponMeshComponent()->SetSkeletalMesh(
			CharacterPlayerState->GetThirdWeaponInfo().WeaponMesh);
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
}