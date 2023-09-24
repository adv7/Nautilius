// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());

	TArray<UClimbComponent*> ClimbComponents;
	GetComponents<UClimbComponent*>(OUT ClimbComponents);

	if (!ClimbComponents.IsEmpty())
		ClimbComponent = ClimbComponents[0];
	else return;

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSprinting) Sprint();

	if (ensure(PlayerController)) SetHeadShake();

	bIsFalling = (GetVelocity().Z < 0.f) ? true : false;
	if (bIsFalling && !ClimbComponent->bIsClimbing) ClimbComponent->TryClimb();

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayerCharacter::OvercomeObstacle);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &APlayerCharacter::StartSprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &APlayerCharacter::StopSprint);
}

void APlayerCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);	
	bIsMovingBackOrForth = (AxisValue != 0.f) ? true : false;
	bIsMovingForward = (AxisValue > 0.f) ? true : false;
}

void APlayerCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
	bIsMovingSideways = (AxisValue != 0.f) ? true : false;
}

void APlayerCharacter::OvercomeObstacle()
{
	ACharacter::Jump();
	ClimbComponent->TryClimb();
}

void APlayerCharacter::Shoot()
{
	UE_LOG(LogTemp, Warning, TEXT("Shoot"));
}

void APlayerCharacter::StartSprint()
{
	bIsSprinting = true;
}

void APlayerCharacter::Sprint()
{
	if (bIsMovingForward && !bIsFalling) GetCharacterMovement()->MaxWalkSpeed = SprintVelocity;
	else GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
}

void APlayerCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
	bIsSprinting = false;
}

void APlayerCharacter::SetHeadShake()
{
	if (ensure(IdleHeadShake) && ensure(WalkHeadShake) && ensure(SprintHeadShake))
	{
		if (bIsSprinting && bIsMovingForward)
		{
			StopCameraShakes(TArray<UCameraShakeBase*> {IdleHeadShakeInstance, WalkHeadShakeInstance});
			SprintHeadShakeInstance = PlayerController->PlayerCameraManager->StartCameraShake(SprintHeadShake);
		}
		else if (bIsMovingBackOrForth || bIsMovingSideways)
		{
			StopCameraShakes(TArray<UCameraShakeBase*> {IdleHeadShakeInstance, SprintHeadShakeInstance});
			WalkHeadShakeInstance = PlayerController->PlayerCameraManager->StartCameraShake(WalkHeadShake);
		}
		else
		{
			StopCameraShakes(TArray<UCameraShakeBase*> {WalkHeadShakeInstance, SprintHeadShakeInstance});
			IdleHeadShakeInstance = PlayerController->PlayerCameraManager->StartCameraShake(IdleHeadShake);
		}
	}
}

/* Shakes work fine without stopping them, leaved as a form of fun fact. */
void APlayerCharacter::StopCameraShakes(TArray<UCameraShakeBase*> CameraShakeInstances)
{
	for (UCameraShakeBase* CameraShakeInstance : CameraShakeInstances)
	{
		if (CameraShakeInstance) 
			PlayerController->PlayerCameraManager->StopCameraShake(CameraShakeInstance, false);
	}
}
