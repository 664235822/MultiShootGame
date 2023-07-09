// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiShootGamePlayerState.h"
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

void AMultiShootGamePlayerState::SetMainWeaponMesh_Server_Implementation(USkeletalMesh* WeaponMesh)
{
	MainWeaponMesh = WeaponMesh;
}

void AMultiShootGamePlayerState::SetSecondWeaponMesh_Server_Implementation(USkeletalMesh* WeaponMesh)
{
	SecondWeaponMesh = WeaponMesh;
}

void AMultiShootGamePlayerState::SetThirdWeaponMesh_Server_Implementation(USkeletalMesh* WeaponMesh)
{
	ThirdWeaponMesh = WeaponMesh;
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
