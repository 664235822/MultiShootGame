// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiShootGamePlayerState.h"

#include "MultiShootGame/Character/MultiShootGameCharacter.h"
#include "Net/UnrealNetwork.h"

void AMultiShootGamePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiShootGamePlayerState, Kill);
	DOREPLIFETIME(AMultiShootGamePlayerState, Death);
	DOREPLIFETIME(AMultiShootGamePlayerState, MainWeaponMesh);
	DOREPLIFETIME(AMultiShootGamePlayerState, SecondWeaponMesh);
	DOREPLIFETIME(AMultiShootGamePlayerState, ThirdWeaponMesh);
}

void AMultiShootGamePlayerState::HandleWeaponMesh_Server_Implementation()
{
	const AMultiShootGameCharacter* Character = Cast<AMultiShootGameCharacter>(GetPawn());
	if (Character)
	{
		MainWeaponMesh = Character->GetCurrentMainWeapon()->WeaponInfo.WeaponMesh;
		SecondWeaponMesh = Character->GetCurrentSecondWeapon()->WeaponInfo.WeaponMesh;
		ThirdWeaponMesh = Character->GetCurrentThirdWeapon()->WeaponInfo.WeaponMesh;
	}
}

void AMultiShootGamePlayerState::AddScore_Server_Implementation(int Num)
{
	SetScore(GetScore() + Num);
}

void AMultiShootGamePlayerState::AddKill_Server_Implementation(int Num)
{
	Kill += Num;
}

void AMultiShootGamePlayerState::AddDeath_Server_Implementation(int Num)
{
	Death += Num;
}
