// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MultiShootGameHUD.generated.h"

UCLASS()
class AMultiShootGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMultiShootGameHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

