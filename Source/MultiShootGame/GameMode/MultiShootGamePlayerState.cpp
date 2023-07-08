// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiShootGamePlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "MultiShootGame/Character/MultiShootGameCharacter.h"
#include "Net/UnrealNetwork.h"

void AMultiShootGamePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiShootGamePlayerState, Kill);
	DOREPLIFETIME(AMultiShootGamePlayerState, Death);
	DOREPLIFETIME(AMultiShootGamePlayerState, MainWeaponInfo);
	DOREPLIFETIME(AMultiShootGamePlayerState, SecondWeaponInfo);
	DOREPLIFETIME(AMultiShootGamePlayerState, ThirdWeaponInfo);
}

void AMultiShootGamePlayerState::SetMainWeaponInfo_Server_Implementation(FWeaponInfo WeaponInfo)
{
	MainWeaponInfo = WeaponInfo;
}

void AMultiShootGamePlayerState::SetSecondWeaponInfo_Server_Implementation(FWeaponInfo WeaponInfo)
{
	SecondWeaponInfo = WeaponInfo;
}

void AMultiShootGamePlayerState::SetThirdWeaponInfo_Server_Implementation(FWeaponInfo WeaponInfo)
{
	ThirdWeaponInfo = WeaponInfo;
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
