// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiShootGameGameMode.h"
#include "MultiShootGameHUD.h"
#include "MultiShootGameCharacter.h"
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

	check(DefaultUserWidgetClass);

	CurrentUserWidget = CreateWidget(GetWorld(), DefaultUserWidgetClass);
	CurrentUserWidget->AddToViewport();
}

void AMultiShootGameGameMode::ToggleAimWidget(bool SniperAimed)
{
	CurrentUserWidget->RemoveFromParent();
	if (SniperAimed)
	{
		CurrentUserWidget = CreateWidget(GetWorld(), SniperUserWidgetClass);
		CurrentUserWidget->AddToViewport();
	}
	else
	{
		CurrentUserWidget = CreateWidget(GetWorld(), DefaultUserWidgetClass);
		CurrentUserWidget->AddToViewport();
	}
}
