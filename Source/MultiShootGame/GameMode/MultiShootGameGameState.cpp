// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiShootGameGameState.h"

void AMultiShootGameGameState::SetWaveState(EWaveState NewWaveState)
{
	const EWaveState OldState = WaveState;
	WaveState = NewWaveState;
}