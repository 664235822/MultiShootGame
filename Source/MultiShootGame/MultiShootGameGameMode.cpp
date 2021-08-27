// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiShootGameGameMode.h"
#include "MultiShootGameHUD.h"
#include "MultiShootGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMultiShootGameGameMode::AMultiShootGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AMultiShootGameHUD::StaticClass();
}
