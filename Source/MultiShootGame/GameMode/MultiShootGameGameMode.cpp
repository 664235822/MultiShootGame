// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiShootGameGameMode.h"

#include "MultiShootGameGameInstance.h"
#include "MultiShootGameGameState.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MultiShootGame/Character/MultiShootGameCharacter.h"
#include "MultiShootGame/Component/HealthComponent.h"

AMultiShootGameGameMode::AMultiShootGameGameMode()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;

	GameStateClass = AMultiShootGameGameState::StaticClass();
	PlayerStateClass = AMultiShootGameGameState::StaticClass();
}

void AMultiShootGameGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AMultiShootGameGameMode::StartPlay()
{
	Super::StartPlay();

	PrepareForNextWave();
}

void AMultiShootGameGameMode::StartWave()
{
	WaveCount++;

	NumberOfBotsToSpawn = 2 * WaveCount;

	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &AMultiShootGameGameMode::SpawnBotTimerElapsed, 1.0f,
	                                true,
	                                0.0f);

	SetWaveState(EWaveState::WaveInProgress);
}

void AMultiShootGameGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);

	SetWaveState(EWaveState::WaitingToComplete);
}

void AMultiShootGameGameMode::PrepareForNextWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &AMultiShootGameGameMode::StartWave,
	                                TimeBetweenWaves, false);

	SetWaveState(EWaveState::WaitingToStart);

	RespawnDeadPlayers();
}

void AMultiShootGameGameMode::SpawnBotTimerElapsed()
{
	SpawnNewBot();

	NumberOfBotsToSpawn--;

	if (NumberOfBotsToSpawn <= 0)
	{
		EndWave();
	}
}

void AMultiShootGameGameMode::GameOver()
{
	EndWave();

	SetWaveState(EWaveState::GameOver);
}

void AMultiShootGameGameMode::CheckWaveState()
{
	if (NumberOfBotsToSpawn > 0 || GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart))
	{
		return;
	}

	bool bIsAnyBotAlive = false;

	for (FConstPawnIterator Iterator = GetWorld()->GetPawnIterator(); Iterator; ++Iterator)
	{
		APawn* TestPawn = Iterator->Get();
		if (TestPawn == nullptr || TestPawn->IsPlayerControlled())
		{
			continue;
		}

		UHealthComponent* HealthComponent = Cast<UHealthComponent>(
			TestPawn->GetComponentByClass(UHealthComponent::StaticClass()));

		if (HealthComponent && HealthComponent->GetHealth() > 0)
		{
			bIsAnyBotAlive = true;
			break;
		}
	}

	if (!bIsAnyBotAlive)
	{
		SetWaveState(EWaveState::WaveComplete);

		PrepareForNextWave();
	}
}

void AMultiShootGameGameMode::CheckAnyPlayerAlive()
{
	const AMultiShootGameCharacter* Character = Cast<AMultiShootGameCharacter>(
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Character)
	{
		if (Character->GetHealthComponent()->bDied)
		{
			GameOver();
			return;
		}
	}

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		if (PlayerController && PlayerController->GetPawn())
		{
			APawn* PlayerPawn = PlayerController->GetPawn();
			UHealthComponent* HealthComponent = Cast<UHealthComponent>(
				PlayerPawn->GetComponentByClass(UHealthComponent::StaticClass()));
			if (ensure(HealthComponent) && HealthComponent->GetHealth() > 0.0f)
			{
				return;
			}
		}
	}

	GameOver();
}

void AMultiShootGameGameMode::CheckNumberOfBots()
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMultiShootGameEnemyCharacter::StaticClass(), OutActors);
	int Count = 0;
	for (int i = 0; i < OutActors.Num(); i++)
	{
		AMultiShootGameEnemyCharacter* EnemyCharacter = Cast<AMultiShootGameEnemyCharacter>(OutActors[i]);
		if (Cast<UHealthComponent>(EnemyCharacter->GetComponentByClass(UHealthComponent::StaticClass()))->bDied)
		{
			continue;
		}
		Count++;
	}
	NumberOfBots = Count;
}

void AMultiShootGameGameMode::SetWaveState(EWaveState NewState) const
{
	AMultiShootGameGameState* MyGameState = GetGameState<AMultiShootGameGameState>();
	if (ensureAlways(MyGameState))
	{
		MyGameState->SetWaveState(NewState);
	}
}

EWaveState AMultiShootGameGameMode::GetWaveState() const
{
	const AMultiShootGameGameState* MyGameState = GetGameState<AMultiShootGameGameState>();

	return MyGameState->GetWaveState();
}

void AMultiShootGameGameMode::RespawnDeadPlayers()
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		if (PlayerController && PlayerController->GetPawn() == nullptr)
		{
			RestartPlayer(PlayerController);
		}
	}
}

void AMultiShootGameGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetWaveState() == EWaveState::GameOver)
	{
		return;
	}

	CheckWaveState();

	CheckAnyPlayerAlive();

	CheckNumberOfBots();
}
