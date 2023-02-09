// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiShootGame/Gamemode/MultiShootGamePlayerState.h"
#include "GameFramework/GameState.h"
#include "MultiShootGameServerGameState.generated.h"

/**
 * 
 */
UCLASS()
class MULTISHOOTGAME_API AMultiShootGameServerGameState : public AGameState
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated)
	TArray<AMultiShootGamePlayerState*> PlayerStates;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void AddPlayer_Server(AMultiShootGamePlayerState* PlayerState);

	UFUNCTION(Server, Reliable)
	void RemovePlayer_Server(AMultiShootGamePlayerState* PlayerState);
};
