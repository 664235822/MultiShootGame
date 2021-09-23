// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_DistanceToCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Kismet/KismetMathLibrary.h"

UBTDecorator_DistanceToCharacter::UBTDecorator_DistanceToCharacter(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Distance To Character";
	BlackboardKey.AddObjectFilter(this,GET_MEMBER_NAME_CHECKED(UBTDecorator_DistanceToCharacter, BlackboardKey),
	                              AActor::StaticClass());

	BlackboardKey.SelectedKeyName = FBlackboard::KeySelf;

	Radius = 1000.f;
}

void UBTDecorator_DistanceToCharacter::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);

	
}

void UBTDecorator_DistanceToCharacter::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskDeactivated(Task);
}

bool UBTDecorator_DistanceToCharacter::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
                                                                  uint8* NodeMemory) const
{
	const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent)
	{
		return false;
	}

	AActor* Character = Cast<AActor>(
		BlackboardComponent->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID()));
	if (!Character)
	{
		return false;
	}

	const AController* Controller = Cast<AController>(OwnerComp.GetOwner());
	APawn* Pawn = Controller ? Controller->GetPawn() : nullptr;
	if (!Pawn)
	{
		return false;
	}

	float Distance = FVector::Dist(Character->GetActorLocation(), Pawn->GetActorLocation());

	return UKismetMathLibrary::LessEqual_FloatFloat(Distance, Radius);
}
