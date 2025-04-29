// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interactable.h"
#include "InteractionPromptWidget.h"
#include "PlayerCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;

UCLASS()
class CELESTIAL_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	APlayerCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Input functions called by PlayerController
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void Turn(float Value);
	void StartSprint();
	void StopSprint();
	UFUNCTION(BlueprintNativeEvent, Category = "Movement")
	void PlayDashMontage();
	UFUNCTION(BlueprintCallable)
	void Dash();
	void DashReload();
	void StartCrouch();
	void StopCrouch();
	void Interact();

	FVector DefaultCameraLocation;
	FRotator DefaultCameraRotation;
	float DefaultCameraBoomLength;
	//for pick ups
	void AddKey(FName Key);

	bool HasKey(FName KeyID) const;

	void TryInteract();
	
	void ResetCameraAfterInspect();
	
	void SaveCameraDefaults();
	FVector DefaultCameraBoomLocation;
	FRotator DefaultCameraBoomRotation;


	FHitResult HitResult;

	UPROPERTY(BlueprintReadOnly)
	int CollectedCollectables = 0;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float MaximumInteractionDistance = 750.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float InteractionRadius = 30.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	bool bToggleDebugVisualiser = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FVector SweepStartOffset = FVector(0.f, 0.f, 50.f); // Example: Start from head height

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UInteractionPromptWidget> InteractionPromptClass;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> CollectableCounterClass;
	UInteractionPromptWidget* InteractionPromptInstance;
	UUserWidget* CollectableCounterInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float LookUpSensitivity = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float TurnSensitivity = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float SprintMultiplier = 2.0f; // Multiplier for sprinting
	float DefaultWalkSpeed; // Stores original MaxWalkSpeed
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float DashDistance = 1500.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float VerticalDashDistance = 100.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float DashCooldown = 0.5f;  // Cooldown time in seconds
	bool bCanDash = true;
	FTimerHandle DashCooldownTimerHandle;



	AInteractable* LastInteractable;
private:
	float DeadZoneThreshold = 0.1f;

	//Zoom 
	FVector OriginalCameraLocation;
	FRotator OriginalCameraRotation;
	bool bIsInspecting = false;
	float DefaultArmLength = 300.f;
	bool bIsExitingInspection = false;
	FVector DefaultCameraBoomWorldLocation;
	FRotator DefaultCameraBoomWorldRotation;

	FVector TargetInspectLocation;
	FRotator TargetInspectRotation;
	bool bIsResettingCamera = false;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TSet<FName> CollectedKeys;
	bool bCameraDefaultsSaved = false;

	float CameraResetTimer = 0.f;
	float CameraResetDuration = 0.5f; // Half second to reset
	float StartingArmLength;
	FRotator StartingBoomRotation;

};
