// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MultiShootGame/Enum/EWaveState.h"
#include "MultiShootGameGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, VictimActor, AActor*, KillerActor, AController*,
											   KillerController);

UCLASS(minimalapi)
class AMultiShootGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMultiShootGameGameMode();

	virtual void StartPlay() override;

protected:
	// Called when the game starts or when spawned
    virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Game Mode")
	void SpawnNewBot();

	void SpawnBotTimerElapsed();

	void StartWave();

	void EndWave();

	void PrepareForNextWave();

	void GameOver();

	FTimerHandle TimerHandle_BotSpawner;

	FTimerHandle TimerHandle_NextWaveStart;

	int32 NumberOfBotsToSpawn;

	int32 WaveCount;

	UPROPERTY(EditDefaultsOnly, Category = "Game Mode")
	float TimeBetweenWaves = 2.0f;

	void CheckWaveState();

	void CheckAnyPlayerAlive();

	void SetWaveState(EWaveState NewState);

	void RespawnDeadPlayers();

public:

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintAssignable, Category = "Game Mode")
	FOnActorKilled OnActorKilled;
};



