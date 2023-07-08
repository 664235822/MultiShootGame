// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MultiShootGame/Struct/WeaponInfo.h"
#include "MultiShootGamePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MULTISHOOTGAME_API AMultiShootGamePlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated)
	int Kill;

	UPROPERTY(Replicated)
	int Death;

	UPROPERTY(Replicated)
	FWeaponInfo MainWeaponInfo;

	UPROPERTY(Replicated)
	FWeaponInfo SecondWeaponInfo;

	UPROPERTY(Replicated)
	FWeaponInfo ThirdWeaponInfo;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable, Category = PlayerState)
	void SetMainWeaponInfo_Server(FWeaponInfo WeaponInfo);

	UFUNCTION(Server, Reliable, Category = PlayerState)
	void SetSecondWeaponInfo_Server(FWeaponInfo WeaponInfo);

	UFUNCTION(Server, Reliable, Category = PlayerState)
	void SetThirdWeaponInfo_Server(FWeaponInfo WeaponInfo);

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

	UFUNCTION(BlueprintPure, Category = PlayerState)
	FORCEINLINE FWeaponInfo GetMainWeaponInfo() const { return MainWeaponInfo; }

	UFUNCTION(BlueprintPure, Category = PlayerState)
	FORCEINLINE FWeaponInfo GetSecondWeaponInfo() const { return SecondWeaponInfo; }

	UFUNCTION(BlueprintPure, Category = PlayerState)
	FORCEINLINE FWeaponInfo GetThirdWeaponInfo() const { return ThirdWeaponInfo; }
};
