// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MultiShootGameServerGameState.generated.h"

/**
 * 
 */
UCLASS()
class MULTISHOOTGAME_API AMultiShootGameServerGameState : public AGameState
{
	GENERATED_BODY()

public:
	UFUNCTION(Server, Reliable, Category = ServerGameState)
	void HandleCharacterWeaponMesh_Server();

protected:
	UFUNCTION(NetMulticast, Reliable, Category = ServerGameState)
	void HandleCharacterWeaponMesh_Multicast();
};
