// Fill out your copyright notice in the Description page of Project Settings.

#include "ClimbComponent.h"

// Sets default values for this component's properties
UClimbComponent::UClimbComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UClimbComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<UBoxComponent*> boxComponents;
	GetOwner()->GetComponents<UBoxComponent>(OUT boxComponents);
	
	FName componentName;
	for (UBoxComponent* boxComponent : boxComponents)
	{
		if (boxComponent->GetFName() == obstacleDetectorVolumeName)
			obstacleDetector = boxComponent;
	}

	if (obstacleDetector == nullptr)
		return;
	
}


// Called every frame
void UClimbComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UClimbComponent::Climb()
{
	TArray<AActor*> detectedActors{ nullptr };
	obstacleDetector->GetOverlappingActors(OUT detectedActors);

	if (!detectedActors.IsEmpty())
	{
		AActor* detectedActor = detectedActors[0];

		FBox detectedActorCollisionBoundings = detectedActor->GetComponentsBoundingBox();
		UE_LOG(LogTemp, Display, TEXT("Climb: %s, bounings: %s"), *detectedActor->GetName(), *detectedActorCollisionBoundings.ToString());

		FVector climbDestinationLocation = GetOwner()->GetActorLocation();
		climbDestinationLocation.Z = detectedActorCollisionBoundings.Max.Z;

		GetOwner()->SetActorLocation(climbDestinationLocation);
	}

}

