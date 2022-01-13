// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_PawnActionBase.h"
#include "BTTask_Destroy.generated.h"

/**
 * 
 */
UCLASS()
class MULTISHOOTGAME_API UBTTask_Destroy : public UBTTask_PawnActionBase
{
	GENERATED_BODY()

protected:
	UBTTask_Destroy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
