// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MultiShootGame/Enum/EGameTime.h"
#include "MultiShootGameGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MULTISHOOTGAME_API UMultiShootGameGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = GameInstance)
	EGameTime GameTime = EGameTime::Daytime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = GameInstance)
	bool bRaining = false;
};
