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
	USkeletalMesh* MainWeaponMesh;

	UPROPERTY(Replicated)
	USkeletalMesh* SecondWeaponMesh;

	UPROPERTY(Replicated)
	USkeletalMesh* ThirdWeaponMesh;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable, Category = PlayerState)
	void SetMainWeaponMesh_Server(USkeletalMesh* WeaponMesh);

	UFUNCTION(Server, Reliable, Category = PlayerState)
	void SetSecondWeaponMesh_Server(USkeletalMesh* WeaponMesh);

	UFUNCTION(Server, Reliable, Category = PlayerState)
	void SetThirdWeaponMesh_Server(USkeletalMesh* WeaponMesh);

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
	FORCEINLINE USkeletalMesh* GetMainWeaponMesh() const { return MainWeaponMesh; }

	UFUNCTION(BlueprintPure, Category = PlayerState)
	FORCEINLINE USkeletalMesh* GetSecondWeaponMesh() const { return SecondWeaponMesh; }

	UFUNCTION(BlueprintPure, Category = PlayerState)
	FORCEINLINE USkeletalMesh* GetThirdWeaponMesh() const { return ThirdWeaponMesh; }
};
