// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MULTISHOOTGAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, UHealthComponent*, OwningHealthComponent,
	                                             float, Health, float, HealthDelta, const class UDamageType*,
	                                             DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeadShotSignature, AActor*, DamageCauser);

public:
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Health)
	float DefaultHealth = 100.f;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Health)
	float CurrentHealth = DefaultHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Health)
	UAnimMontage* HitHeadMontage;

	UFUNCTION(BlueprintCallable)
	void Heal(float HealAmount);

	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	                         class AController* InstigatedBy, AActor* DamageCauser);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = Health)
	bool bDied = false;

	UFUNCTION(BlueprintPure, Category = Health)
	FORCEINLINE float GetHealth() const { return CurrentHealth; }

	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnHeadShotSignature OnHeadShot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Health)
	uint8 TeamNumber = 0;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Health)
	static bool IsFriendly(AActor* ActorA, AActor* ActorB);
};
