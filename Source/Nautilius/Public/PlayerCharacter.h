// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraShakeBase.h"
#include "ClimbComponent.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class NAUTILIUS_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	void OvercomeObstacle();
	void Shoot();

	void StartSprint();
	void Sprint();
	void StopSprint();

	bool bIsSprinting = false;
	bool bIsMovingForward = false;
	bool bIsFalling = false;
	bool bIsMovingBackOrForth = false;
	bool bIsMovingSideways = false;

	APlayerController* PlayerController{ nullptr };
	UClimbComponent* ClimbComponent{ nullptr };

	UCameraShakeBase* IdleHeadShakeInstance{ nullptr };
	UCameraShakeBase* WalkHeadShakeInstance{ nullptr };
	UCameraShakeBase* SprintHeadShakeInstance{ nullptr };
	void SetHeadShake();
	void StopCameraShakes(TArray<UCameraShakeBase*> CameraShakeInstances);

public:
	/** Speed in centimeters per second. */
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float DefaultWalkSpeed = 388.f;

	/** Speed in centimeters per second. */
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float SprintVelocity = 667.f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TSubclassOf<UCameraShakeBase> IdleHeadShake{ nullptr };

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TSubclassOf<UCameraShakeBase> WalkHeadShake{ nullptr };

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TSubclassOf<UCameraShakeBase> SprintHeadShake{ nullptr };

};
