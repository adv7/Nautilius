// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CapsuleComponent.h"
#include "ClimbComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NAUTILIUS_API UClimbComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UClimbComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, Category="Climb")
	float MaxRange = 150.f;

	UPROPERTY(EditDefaultsOnly, Category="Climb")
	float SurfaceDetectionRadius = 30.f;

	UPROPERTY(EditDefaultsOnly, Category = "Climb")
	float UpDistanceAfterClimb = 10.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Climb")
	float ForwardDistanceAfterClimb = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "Climb")
	float ClimbSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Climb")
	float AvailableSlopeAngle = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Climb")
	float AvailableClimbHeight = 60.f;

	/** Time after climb when performing climb againg is impossible */
	UPROPERTY(EditDefaultsOnly, Category = "Climb")
	float AfterClimbRestTime = 0.5f;

	bool bIsClimbing = false;
	void TryClimb();

private:
	AController* OwnerController{ nullptr };
	UCapsuleComponent* OwnerCapsule{ nullptr };
	float PlayerCapsuleHeight = 0.f;
	float PlayerCapsuleRadius = 0.f;
	
	FVector ClimbStartLocation = FVector::ZeroVector;
	FVector ClimbEndLocation = FVector::ZeroVector;
	FVector ForwardEndLocation = FVector::ZeroVector;
	float ClimbProgress = 0.f;
	float ClimbLength = 0.f;
	float MoveForwardProgress = 0.f;
	float AfterClimbRestProgress = 0.f;

	const float CheckingDownwardDistance = 5.f;

	void Climb(float DeltaTime);
		
};
