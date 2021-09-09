// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MultiShootGameGameMode.generated.h"

UCLASS(minimalapi)
class AMultiShootGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMultiShootGameGameMode();

protected:
	// Called when the game starts or when spawned
    virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "GameMode")
	TSubclassOf<UUserWidget> UserWidgetClass;

	UUserWidget* CurrentUserWidget;
};



