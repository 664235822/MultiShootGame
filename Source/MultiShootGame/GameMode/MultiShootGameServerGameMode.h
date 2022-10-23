// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MultiShootGameServerGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MULTISHOOTGAME_API AMultiShootGameServerGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMultiShootGameServerGameMode();

	virtual void StartPlay() override;
};
