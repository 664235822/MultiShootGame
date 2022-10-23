// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MultiShootGameGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MULTISHOOTGAME_API AMultiShootGameGameModeBase : public AGameMode
{
	GENERATED_BODY()

public:
	AMultiShootGameGameModeBase();

	virtual void StartPlay() override;

protected:
	void CheckShowSight(float DeltaSeconds);

	UFUNCTION(BlueprintCallable)
	void Clear();

	float CurrentShowSight = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = GameMode)
	float ShowSightDelay = 1.f;

	UPROPERTY(BlueprintReadOnly)
	bool bShowSight = false;

	UPROPERTY(BlueprintReadOnly)
	int Score;

	UPROPERTY(BlueprintReadOnly)
	int KillCount;

	UPROPERTY(BlueprintReadOnly)
	int DeathCount;
public:
	virtual void Tick(float DeltaSeconds) override;

	void OnEnemyKilled();

	void OnHeadshot();

	void OnDeath();
	
};
