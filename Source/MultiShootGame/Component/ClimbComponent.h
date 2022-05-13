// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClimbComponent.generated.h"

class AMultiShootGameCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MULTISHOOTGAME_API UClimbComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UClimbComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void EndJump();

private:
	AMultiShootGameCharacter* OwnerCharacter;

	FVector CharacterLocation;

	FVector CharacterForwardVector;

	FVector WallLocation;

	FVector WallNormal;
	
	bool bDetectClimbing = false;

	float HalfHeight;

	float WallHeight;

	bool bLastDetect;

	float DetectDistance = 110.f;

	int StartHeight;

	int DetectHeight = 15;

	float AngleWithWall;

	bool bCanJump = true;

	bool bMoving;

	void SlopeDetecting();

	void StairHeightDetecting(int Index);

	bool CanGetFrontOfClimbing();

	bool WallWeightDetecting();

	bool ClimbingDetecting(float Multiply);

	void ClimbStart();

	void VaultStart();

	void ClimbCalculate(float Distance, float Height, FVector& Location, FRotator& Rotation);

	void ClimbCalculateMesh(float Distance, float Height, FVector& Location, FRotator& Rotation);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void BeginJump();

	bool GetDetectClimbing() const;
};
