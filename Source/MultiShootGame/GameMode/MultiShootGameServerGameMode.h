// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiShootGameGameInstance.h"
#include "MultiShootGameGameModeBase.h"
#include "GameFramework/GameMode.h"
#include "MultiShootGameServerGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MULTISHOOTGAME_API AMultiShootGameServerGameMode : public AMultiShootGameGameModeBase
{
	GENERATED_BODY()

public:
	AMultiShootGameServerGameMode();

	virtual void StartPlay() override;
	
};
