#pragma once

UENUM(BlueprintType)
enum class EWeaponMode : uint8
{
	Weapon UMETA(DisplayName = "Weapon"),
	Sniper UMETA(DisplayName = "Sniper"),
	Shotgun UMETA(DisplayName = "Shotgun")
};