// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiShootGamePlayerState.h"
#include "Net/UnrealNetwork.h"

void AMultiShootGamePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiShootGamePlayerState, Kill);
	DOREPLIFETIME(AMultiShootGamePlayerState, Death);
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
