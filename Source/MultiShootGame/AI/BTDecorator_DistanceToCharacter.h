// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_DistanceToCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MULTISHOOTGAME_API UBTDecorator_DistanceToCharacter : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

protected:
	UBTDecorator_DistanceToCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, Category="Distance")
	float Radius;
};
