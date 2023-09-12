// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
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
	float MaxRange = 200;

	UPROPERTY(EditDefaultsOnly, Category = "Climb")
	float UpDistanceAfterClimb = 10;
	
	UPROPERTY(EditDefaultsOnly, Category = "Climb")
	float ForwardDistanceAfterClimb = 50;

	UPROPERTY(EditDefaultsOnly, Category = "Climb")
	float ClimbSpeed = 100;

	UPROPERTY(EditDefaultsOnly, Category = "Climb")
	float AvailableSlopeAngle = 40;

	bool bClimb = false;
	void CalculateClimbDestination();

private:
	AController* OwnerController{ nullptr };
	
	FVector ClimbStartLocation = FVector::ZeroVector;
	FVector ClimbEndLocation = FVector::ZeroVector;
	float ClimbProgress = 0.f;
	float ClimbLength = 0.f;

	void Climb(float DeltaTime);
		
};
