// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MultiShootGame/Enum/EWaveState.h"
#include "MultiShootGameGameState.generated.h"

/**
 * 
 */
UCLASS()
class MULTISHOOTGAME_API AMultiShootGameGameState : public AGameState
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = GameState)
	EWaveState WaveState;

public:
	UFUNCTION(BlueprintCallable, Category = GameState)
	void SetWaveState(EWaveState NewWaveState);

	UFUNCTION(BlueprintPure, Category = GameState)
	FORCEINLINE EWaveState GetWaveState() const { return WaveState; }
};
