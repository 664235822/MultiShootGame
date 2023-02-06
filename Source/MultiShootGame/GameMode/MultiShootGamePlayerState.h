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

public:
	UFUNCTION(Server, Reliable, Category = PlayerState)
	void AddScore_Server(int Num);

	UFUNCTION(Server, Reliable, Category = PlayerState)
	void AddKill_Server(int Num);

	UFUNCTION(Server, Reliable, Category = PlayerState)
	void AddDeath_Server(int Num);

	UFUNCTION(BlueprintPure, Category = PlayerState)
	FORCEINLINE int GetKill() const { return Kill; }

	UFUNCTION(BlueprintPure, Category = PlayerState)
	FORCEINLINE int GetDeath() const { return Death; }

protected:
	UPROPERTY(Replicated)
	int Kill;

	UPROPERTY(Replicated)
	int Death;

	virtual void ClientInitialize(AController* C) override;

	virtual void Destroyed() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
