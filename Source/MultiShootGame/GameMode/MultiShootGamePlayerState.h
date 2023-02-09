// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MultiShootGamePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MULTISHOOTGAME_API AMultiShootGamePlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	virtual void ClientInitialize(AController* C) override;

	virtual void Destroyed() override;

	UPROPERTY(Replicated)
	int Kill;

	UPROPERTY(Replicated)
	int Death;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(Server, Reliable, Category = PlayerState)
	void AddScore_Server(int Num = 1);

	UFUNCTION(Server, Reliable, Category = PlayerState)
	void AddKill_Server(int Num = 1);

	UFUNCTION(Server, Reliable, Category = PlayerState)
	void AddDeath_Server(int Num = 1);

	UFUNCTION(BlueprintPure, Category = PlayerState)
	FORCEINLINE int GetKill() const { return Kill; }

	UFUNCTION(BlueprintPure, Category = PlayerState)
	FORCEINLINE int GetDeath() const { return Death; }
};
