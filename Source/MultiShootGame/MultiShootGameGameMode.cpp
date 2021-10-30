// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiShootGameGameMode.h"
#include "MultiShootGameHUD.h"
#include "Character/MultiShootGameCharacter.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

AMultiShootGameGameMode::AMultiShootGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(
		TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AMultiShootGameHUD::StaticClass();
}

void AMultiShootGameGameMode::BeginPlay()
{
	Super::BeginPlay();
}

