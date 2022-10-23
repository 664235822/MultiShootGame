// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiShootGameGameState.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MultiShootGame/Character/MultiShootGameCharacter.h"

void AMultiShootGameGameState::SetWaveState(EWaveState NewWaveState)
{
	const EWaveState OldState = WaveState;
	WaveState = NewWaveState;
}
