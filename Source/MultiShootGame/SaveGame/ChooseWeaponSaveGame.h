// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MultiShootGame/Struct/WeaponInfo.h"
#include "ChooseWeaponSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class MULTISHOOTGAME_API UChooseWeaponSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ChooseWeapon")
	TArray<FWeaponInfo> MainWeaponList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ChooseWeapon")
	TArray<FWeaponInfo> SecondWeaponList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ChooseWeapon")
	TArray<FWeaponInfo> ShotgunWeaponList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ChooseWeapon")
	int MainWeaponIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ChooseWeapon")
	int SecondWeaponIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ChooseWeapon")
	int ShotgunWeaponIndex;
};
