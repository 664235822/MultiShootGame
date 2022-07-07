#pragma once

UENUM(BlueprintType)
enum class EWeaponMode : uint8
{
	MainWeapon UMETA(DisplayName = "MainWeapon"),
	SecondWeapon UMETA(DisplayName = "SecondWeapon"),
	ThirdWeapon UMETA(DisplayName = "ThirdWeapon")
};