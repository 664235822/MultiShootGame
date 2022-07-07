#pragma once

UENUM(BlueprintType)
enum class EGameTime : uint8
{
	Daytime UMETA(DisplayName = "Daytime"),
	Nighttime UMETA(DisplayName = "Nighttime")
};