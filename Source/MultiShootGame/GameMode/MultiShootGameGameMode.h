// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MultiShootGameGameModeBase.h"
#include "MultiShootGame/Enum/EWaveState.h"
#include "MultiShootGameGameMode.generated.h"

UCLASS(minimalapi)
class AMultiShootGameGameMode : public AMultiShootGameGameModeBase
{
	GENERATED_BODY()

public:
	AMultiShootGameGameMode();

	virtual void StartPlay() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = GameMode)
	void SpawnNewBot();

	void SpawnBotTimerElapsed();

	void StartWave();

	void EndWave();

	void PrepareForNextWave();

	void GameOver();

	FTimerHandle TimerHandle_BotSpawner;

	FTimerHandle TimerHandle_NextWaveStart;

	UPROPERTY(BlueprintReadOnly)
	int NumberOfBotsToSpawn;

	UPROPERTY(BlueprintReadOnly)
	int NumberOfBots;

	UPROPERTY(BlueprintReadOnly)
	int WaveCount;

	UPROPERTY(EditDefaultsOnly, Category = GameMode)
	float TimeBetweenWaves = 2.0f;

	void CheckWaveState();

	void CheckAnyPlayerAlive();

	void CheckNumberOfBots();

	void SetWaveState(EWaveState NewState) const;

	UFUNCTION(BlueprintCallable)
	EWaveState GetWaveState() const;

	void RespawnDeadPlayers();

public:
	virtual void Tick(float DeltaSeconds) override;

	
};
