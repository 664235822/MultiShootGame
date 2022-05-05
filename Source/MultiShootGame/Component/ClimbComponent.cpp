// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbComponent.h"
#include "MultiShootGame/MultiShootGame.h"
#include "MultiShootGame/Character/MultiShootGameCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UClimbComponent::UClimbComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UClimbComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AMultiShootGameCharacter>(GetOwner());
}

void UClimbComponent::EndJump()
{
	bDetectClimbing = false;

	OwnerCharacter->SetEnableMovement(true);
}

// 斜坡检测
void UClimbComponent::SlopeDetecting()
{
	const FVector ActorLocation = FVector(CharacterLocation.X, CharacterLocation.Y,
	                                      CharacterLocation.Z - HalfHeight * 0.5f);
	const FVector StartLocation = ActorLocation + CharacterForwardVector * 50.f;
	const FVector EndLocation = ActorLocation + CharacterForwardVector * DetectDistance + FVector(0, 0, 70.f);

	FHitResult HitResult;
	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLocation, EndLocation,TraceType_Visibility, false,
	                                          IgnoreActors, EDrawDebugTrace::Persistent, HitResult, true))
	{
		StartHeight = 0;
	}
	else
	{
		StartHeight = 4;
	}
}

// 楼梯高度检测
void UClimbComponent::StairHeightDetecting(int Index)
{
	const FVector ActorLocation = FVector(CharacterLocation.X, CharacterLocation.Y,
	                                      CharacterLocation.Z - HalfHeight * 0.5 + Index * 20.f);
	const FVector StartLocation = ActorLocation + CharacterForwardVector * 10.f;
	const FVector HalfScale = FVector(0, 10.f, 10.f);
	const FVector EndLocation = ActorLocation + CharacterForwardVector * DetectDistance;
	const FRotator Rotation = UKismetMathLibrary::MakeRotFromX(CharacterForwardVector);
	FHitResult HitResult;
	if (!bLastDetect)
	{
		UKismetSystemLibrary::BoxTraceSingle(GetWorld(), StartLocation, EndLocation, HalfScale, Rotation,
		                                     TraceType_Visibility, false, IgnoreActors, EDrawDebugTrace::Persistent,
		                                     HitResult, true);
	}
	else
	{
		const FVector DetectEndLocation = StartLocation + FVector(WallLocation.X - StartLocation.X,
		                                                          WallLocation.Y - StartLocation.Y, 0) +
			CharacterForwardVector * 20.f;
		UKismetSystemLibrary::BoxTraceSingle(GetWorld(), StartLocation, DetectEndLocation, HalfScale, Rotation,
		                                     TraceType_Visibility, false, IgnoreActors, EDrawDebugTrace::Persistent,
		                                     HitResult, true);
	}

	if (HitResult.bBlockingHit)
	{
		WallLocation = HitResult.Location;
		WallNormal = HitResult.Normal;
		HalfHeight = WallLocation.Z - OwnerCharacter->GetMesh()->GetComponentLocation().Z;
		bLastDetect = true;
	}
	else
	{
		if (bLastDetect)
		{
			bLastDetect = false;
			const FRotator WallForwardRotation = UKismetMathLibrary::MakeRotFromX(-1.f * WallNormal);
			const FVector WallForwardVector = UKismetMathLibrary::GetForwardVector(
				FRotator(0, WallForwardRotation.Yaw, WallForwardRotation.Roll));
			const float DotWithWall = UKismetMathLibrary::Dot_VectorVector(
				CharacterForwardVector, WallForwardVector);
			AngleWithWall = UKismetMathLibrary::DegAcos(DotWithWall);
		}
	}
}

// 能否到达攀爬点前
bool UClimbComponent::CanGetFrontOfClimbing()
{
	const FVector StartLocation = FVector(CharacterLocation.X, CharacterLocation.Y,
	                                      CharacterLocation.Z + HalfHeight * 0.2f);
	FVector EndLocation;
	FRotator EndRotation;
	ClimbCalculate(50, 0, EndLocation, EndRotation);
	EndLocation += FVector(0, 0, HalfHeight * 0.5f);

	FHitResult HitResult;
	if (!UKismetSystemLibrary::CapsuleTraceSingle(GetWorld(), StartLocation, EndLocation, 20.f, 0,
	                                              TraceType_Visibility, false, IgnoreActors,
	                                              EDrawDebugTrace::Persistent, HitResult, true))
	{
		FVector FrontStartLocation;
		FRotator FrontStartRotation;
		ClimbCalculate(50.f, 0, FrontStartLocation, FrontStartRotation);
		FrontStartLocation += FVector(0, 0, HalfHeight);
		FVector FrontEndLocation;
		FRotator FrontEndRotation;
		ClimbCalculate(50.f, WallHeight, FrontEndLocation, FrontEndRotation);
		return UKismetSystemLibrary::SphereTraceSingle(GetWorld(), FrontStartLocation, FrontEndLocation, 20.f,
		                                               TraceType_Visibility, false, IgnoreActors,
		                                               EDrawDebugTrace::Persistent, HitResult, true);
	}

	return false;
}

void UClimbComponent::ClimbCalculate(float Distance, float Height, FVector& Location, FRotator& Rotation)
{
	const FRotator CharacterRotation = OwnerCharacter->GetCapsuleComponent()->GetComponentRotation();
	const FRotator TempRotation = UKismetMathLibrary::MakeRotFromX(
		FVector(-1.f * WallNormal.X, WallNormal.Y, WallNormal.Z));
	Rotation = FRotator(CharacterRotation.Pitch, TempRotation.Yaw, CharacterRotation.Roll);

	const FRotator TempRotation2 = UKismetMathLibrary::MakeRotFromX(WallNormal);

	Location = UKismetMathLibrary::GetForwardVector(FRotator(0, TempRotation2.Yaw, TempRotation2.Roll)) * Distance;

	Location += FVector(WallLocation.X, WallLocation.Y, CharacterLocation.Z + Height);
}

void UClimbComponent::ClimbCalculateMesh(float Distance, float Height, FVector& Location, FRotator& Rotation)
{
	const FRotator CharacterRotation = OwnerCharacter->GetCapsuleComponent()->GetComponentRotation();
	const FRotator TempRotation = UKismetMathLibrary::MakeRotFromX(
		FVector(-1.f * WallNormal.X, WallNormal.Y, WallNormal.Z));
	Rotation = FRotator(CharacterRotation.Pitch, -1.f * TempRotation.Yaw, CharacterRotation.Roll);

	const FRotator TempRotation2 = UKismetMathLibrary::MakeRotFromX(WallNormal);

	Location = UKismetMathLibrary::GetForwardVector(FRotator(0, TempRotation2.Yaw, TempRotation2.Roll)) * Distance;

	const FVector MeshLocation = OwnerCharacter->GetMesh()->GetComponentLocation();
	Location += FVector(WallLocation.X, WallLocation.Y, MeshLocation.Z + Height);
}

// Called every frame
void UClimbComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CharacterLocation = OwnerCharacter->GetActorLocation();
	CharacterForwardVector = OwnerCharacter->GetActorForwardVector();
}

void UClimbComponent::BeginJump()
{
	if (bDetectClimbing)
	{
		return;
	}

	bDetectClimbing = true;

	OwnerCharacter->SetEnableMovement(false);
	HalfHeight = OwnerCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	bLastDetect = false;

	SlopeDetecting();

	for (int i = StartHeight; i <= DetectHeight; i++)
	{
		StairHeightDetecting(i);

		if (AngleWithWall < 50.f)
		{
			if (CanGetFrontOfClimbing())
			{
				if (WallWeightDetecting())
				{
					if (!ClimbingDetecting(2.0f))
					{
						ClimbStart();
						break;
					}
				}
				else
				{
					if (bMoving)
					{
						if (!ClimbingDetecting(1.0f))
						{
							VaultStart();
							break;
						}
					}
					else
					{
						if (ClimbingDetecting(2.0f))
						{
							if (!ClimbingDetecting(1.0f))
							{
								VaultStart();
								break;
							}
						}
						else
						{
							ClimbStart();
							break;
						}
					}
				}
			}
		}
	}

	if (bCanJump)
	{
		OwnerCharacter->NormalJump();
		bDetectClimbing = false;
	}
	else
	{
		bCanJump = true;
	}

	EndJump();
}

// 墙的厚薄检测
bool UClimbComponent::WallWeightDetecting()
{
	FVector StartLocation;
	FRotator StartRotation;
	ClimbCalculate(-60.f, WallHeight, StartLocation, StartRotation);
	FVector EndLocation;
	FRotator EndRotation;
	ClimbCalculateMesh(-60.f, WallHeight, EndLocation, EndRotation);
	EndLocation += FVector(0, 0, 30.f);
	FHitResult HitResult;
	return UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartLocation, EndLocation, 20.F,
	                                               TraceType_Visibility, false, IgnoreActors,
	                                               EDrawDebugTrace::Persistent, HitResult, true);
}

bool UClimbComponent::GetDetectClimbing() const
{
	return bDetectClimbing;
}

// 攀爬翻越判断
bool UClimbComponent::ClimbingDetecting(float Multiply)
{
	FVector Location;
	FRotator Rotation;
	ClimbCalculateMesh(-10.f, WallHeight, Location, Rotation);
	const FVector StartLocation = Location + FVector(0, 0, 40.f);
	const FVector EndLocation = Location + FVector(0, 0, HalfHeight * Multiply);
	FHitResult HitResult;
	return UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartLocation, EndLocation, 20.f,TraceType_Visibility,
	                                               false, IgnoreActors, EDrawDebugTrace::Persistent, HitResult, true);
}

// 攀爬
void UClimbComponent::ClimbStart()
{
	bCanJump = false;
	FVector Location;
	FRotator Rotation;
	ClimbCalculate(50.f, 0, Location, Rotation);
	const FLatentActionInfo LatentActionInfo;
	UKismetSystemLibrary::MoveComponentTo(OwnerCharacter->GetCapsuleComponent(), Location,
	                                      Rotation, false, false, 0.3f, true,
	                                      EMoveComponentAction::Move, LatentActionInfo);
}

// 翻越
void UClimbComponent::VaultStart()
{
	bCanJump = false;
	FVector Location;
	FRotator Rotation;
	ClimbCalculate(50.f, 0, Location, Rotation);
	const FLatentActionInfo LatentActionInfo;
	UKismetSystemLibrary::MoveComponentTo(OwnerCharacter->GetCapsuleComponent(), Location,
	                                      Rotation, false, false, 0.3f, true,
	                                      EMoveComponentAction::Move, LatentActionInfo);
}
