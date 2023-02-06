// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiShootGameServerGameState.h"
#include "Net/UnrealNetwork.h"

void AMultiShootGameServerGameState::AddPlayer_Server_Implementation(AMultiShootGamePlayerState* PlayerState)
{
	PlayerStates.Add(PlayerState);
	for (int i = 0; i < PlayerStates.Num(); i++)
	{
		PlayerStates[i]->SetPlayerId(i);
	}
}

void AMultiShootGameServerGameState::RemovePlayer_Server_Implementation(AMultiShootGamePlayerState* PlayerState)
{
	PlayerStates.Remove(PlayerState);
	for (int i = 0; i < PlayerStates.Num(); i++)
	{
		PlayerStates[i]->SetPlayerId(i);
	}
}

void AMultiShootGameServerGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiShootGameServerGameState, PlayerStates);
}
