#pragma once

UENUM(BlueprintType)
enum class EWaveState : uint8
{
	WaitingToStart UMETA(DisplayName = "WaitingToStart"),
	WaveInProgress UMETA(DisplayName = "WaveInProgress"),
	WaitingToComplete UMETA(DisplayName = "WaitingToComplete"),
	WaveComplete UMETA(DisplayName = "WaveComplete"),
	GameOver UMETA(DisplayName = "GameOver")
};