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

	ToggleDefaultAimWidget(true);
}

void AMultiShootGameGameMode::ToggleDefaultAimWidget(bool Enabled)
{
	if (Enabled)
	{
		CurrentDefaultUserWidget = CreateWidget(GetWorld(), DefaultUserWidgetClass);
		CurrentDefaultUserWidget->AddToViewport();
	}else
	{
		if(CurrentDefaultUserWidget)
		{
			CurrentDefaultUserWidget->RemoveFromParent();
		}
	}
}

void AMultiShootGameGameMode::ToggleSniperAimWidget(bool Enabled)
{
	if (Enabled)
	{
		CurrentSniperUserWidget = CreateWidget(GetWorld(), SniperUserWidgetClass);
		CurrentSniperUserWidget->AddToViewport();
	}
	else
	{
		if(CurrentSniperUserWidget)
		{
			CurrentSniperUserWidget->RemoveFromParent();
		}
	}
}
