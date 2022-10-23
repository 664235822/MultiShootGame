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
	
	virtual void Tick(float DeltaSeconds) override;

	
	
};
