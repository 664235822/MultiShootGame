// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiShootGameGameModeBase.h"

AMultiShootGameGameModeBase::AMultiShootGameGameModeBase(): Super()
{
}

void AMultiShootGameGameModeBase::StartPlay()
{
	Super::StartPlay();
}

void AMultiShootGameGameModeBase::CheckShowSight(float DeltaSeconds)
{
	if (bShowSight)
	{
		if (CurrentShowSight < ShowSightDelay)
		{
			CurrentShowSight += DeltaSeconds;
		}
		else
		{
			bShowSight = false;
			CurrentShowSight = 0.f;
		}
	}
}

void AMultiShootGameGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckShowSight(DeltaSeconds);
}

void AMultiShootGameGameModeBase::OnEnemyKilled()
{
	Score += 50;
	KillCount++;
	bShowSight = true;
	CurrentShowSight = 0.f;
}

void AMultiShootGameGameModeBase::OnHeadshot()
{
	Score += 25;
}

void AMultiShootGameGameModeBase::OnDeath()
{
	DeathCount++;
}

void AMultiShootGameGameModeBase::Clear()
{
	Score = 0;
	KillCount = 0;
	DeathCount = 0;
}
