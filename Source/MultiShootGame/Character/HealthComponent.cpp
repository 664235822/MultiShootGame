// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeAnyDamage);
	}
}


void UHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                           AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || bDied)
	{
		return;
	}

	if (DamageCauser != DamagedActor && IsFriendly(DamageCauser, DamagedActor))
	{
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, DefaultHealth);

	OnHealthChanged.Broadcast(this, CurrentHealth, Damage, DamageType, InstigatedBy, DamageCauser);
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UHealthComponent::GetHealth() const
{
	return CurrentHealth;
}

void UHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0 || CurrentHealth <= 0)
	{
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.0f, DefaultHealth);

	OnHealthChanged.Broadcast(this, CurrentHealth, -HealAmount, nullptr, nullptr, nullptr);
}

bool UHealthComponent::IsFriendly(AActor* ActorA, AActor* ActorB)
{
	if (ActorA == nullptr || ActorB == nullptr)
	{
		return true;
	}

	UHealthComponent* HealthComponentA = Cast<UHealthComponent>(
		ActorA->GetComponentByClass(UHealthComponent::StaticClass()));
	UHealthComponent* HealthComponentB = Cast<UHealthComponent>(
		ActorB->GetComponentByClass(UHealthComponent::StaticClass()));

	if (HealthComponentA == nullptr || HealthComponentB == nullptr)
	{
		return true;
	}

	return HealthComponentA->TeamNumber == HealthComponentB->TeamNumber;
}
