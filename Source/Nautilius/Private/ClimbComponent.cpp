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

	if (bClimb) Climb(DeltaTime);

}

void UClimbComponent::Climb(float DeltaTime)
{
	FVector NewLocation;
	if (ClimbProgress < 1.f)
	{
		const float ClimbStep = (DeltaTime * ClimbSpeed) / ClimbLength;
		ClimbProgress += ClimbStep;
		ClimbProgress = FMath::Clamp<float>(ClimbProgress, 0.f, 1.f);

		NewLocation = FMath::Lerp(ClimbStartLocation, ClimbEndLocation, ClimbProgress);
	}
	else
	{
		bClimb = false;
		ClimbProgress = 0.f;
		ClimbStartLocation = FVector::ZeroVector;
		ClimbEndLocation = FVector::ZeroVector;
		return;
	}
	OwnerController->GetPawn()->SetActorLocation(NewLocation);
	
}

void UClimbComponent::CalculateClimbDestination()
{
	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(OUT Location, OUT Rotation);

	FVector End = Location + Rotation.Vector() * MaxRange;

	FHitResult ClimbDestinationSurface;
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(OUT ClimbDestinationSurface, Location, End, ECollisionChannel::ECC_GameTraceChannel1);
	if (bSuccess)
	{
		if (ClimbDestinationSurface.Normal.Z > cosf(UKismetMathLibrary::DegreesToRadians(AvailableSlopeAngle)))
		{
			ClimbStartLocation = OwnerController->GetPawn()->GetActorLocation();
			
			ClimbEndLocation = ClimbStartLocation;
			ClimbEndLocation.Z += ClimbDestinationSurface.Location.Z + UpDistanceAfterClimb;

			/*FVector PlayerDirection = OwnerController->GetPawn()->GetActorForwardVector();
			ClimbEndLocation.X += ForwardDistanceAfterClimb * PlayerDirection.X;
			ClimbEndLocation.Y += ForwardDistanceAfterClimb * PlayerDirection.Y;*/

			ClimbLength = (ClimbEndLocation - ClimbStartLocation).Size();

			DrawDebugPoint(GetWorld(), ClimbDestinationSurface.Location, 20, FColor::Red, true);
			bClimb = true;
		}
	}
}

