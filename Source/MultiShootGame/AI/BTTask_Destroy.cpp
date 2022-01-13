// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Destroy.h"

UBTTask_Destroy::UBTTask_Destroy(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Destroy";
}

void UBTTask_Destroy::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

void UBTTask_Destroy::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskDeactivated(Task);
}

EBTNodeResult::Type UBTTask_Destroy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AController* Controller = Cast<AController>(OwnerComp.GetOwner());
	APawn* Pawn = Controller ? Controller->GetPawn() : nullptr;
	if (Pawn)
	{
		Pawn->Destroy();

		return EBTNodeResult::Succeeded;
	}

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}