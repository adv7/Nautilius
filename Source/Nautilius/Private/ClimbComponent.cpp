// Fill out your copyright notice in the Description page of Project Settings.

#include "ClimbComponent.h"
#include <Kismet/KismetMathLibrary.h>

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

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;
	OwnerController = OwnerPawn->GetController();
	if (OwnerController == nullptr) return;
	
}


// Called every frame
void UClimbComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UClimbComponent::Climb()
{
	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);

	FVector End = Location + Rotation.Vector() * MaxRange;

	FHitResult ClimbDestinationSurface;
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(OUT ClimbDestinationSurface, Location, End, ECollisionChannel::ECC_GameTraceChannel1);
	if (bSuccess)
	{
		if (ClimbDestinationSurface.Normal.Z > cosf(UKismetMathLibrary::DegreesToRadians(AvailableSlopeAngle)))
		{
			FVector ClimbDestinationLocation = OwnerController->GetPawn()->GetActorLocation();
			ClimbDestinationLocation.Z += ClimbDestinationSurface.Location.Z;
			ClimbDestinationLocation.Z += UpDistanceAfterClimb;

			FVector PlayerDirection = OwnerController->GetPawn()->GetActorForwardVector();
			ClimbDestinationLocation.X += ForwardDistanceAfterClimb * PlayerDirection.X;
			ClimbDestinationLocation.Y += ForwardDistanceAfterClimb * PlayerDirection.Y;

			DrawDebugPoint(GetWorld(), ClimbDestinationSurface.Location, 20, FColor::Red, true);
			OwnerController->GetPawn()->SetActorLocation(ClimbDestinationLocation);
		}
	}

}

