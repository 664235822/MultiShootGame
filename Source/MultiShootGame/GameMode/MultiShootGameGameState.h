// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MultiShootGame/Enum/EWaveState.h"
#include "MultiShootGameGameState.generated.h"

/**
 * 
 */
UCLASS()
class MULTISHOOTGAME_API AMultiShootGameGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = GameState)
	void WaveStateChanged(EWaveState NewState, EWaveState OldState);

	UPROPERTY(BlueprintReadOnly, Category = GameState)
	EWaveState WaveState;

public:
	void SetWaveState(EWaveState NewWaveState);
};
