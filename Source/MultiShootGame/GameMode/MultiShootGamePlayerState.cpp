// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiShootGamePlayerState.h"
#include "MultiShootGame/GameMode/MultiShootGameServerGameState.h"
#include "Net/UnrealNetwork.h"

void AMultiShootGamePlayerState::ClientInitialize(AController* C)
{
	Super::ClientInitialize(C);

	AMultiShootGameServerGameState* GameState = Cast<AMultiShootGameServerGameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		GameState->AddPlayer_Server(this);
	}
}

void AMultiShootGamePlayerState::Destroyed()
{
	Super::Destroyed();

	AMultiShootGameServerGameState* GameState = Cast<AMultiShootGameServerGameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		GameState->RemovePlayer_Server(this);
	}
}

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
