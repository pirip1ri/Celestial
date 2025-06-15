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
class AInspectInteractables;
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

	void UpdateBeam(float DeltaTime);
	AInteractable* HitInteractable;
	void StartBeam();
	void StopBeam();
	bool bIsBeamActive;
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

	void CastLight();
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AInspectInteractables> InspectClass;
	void CastLightEnd();

	bool HasKey(FName KeyID) const;

	void TryInteract();

	void ResetCameraAfterInspect();

	void SaveCameraDefaults();
	void ToggleZoom();
	void UpdateZoom(float DeltaTime);
	FVector DefaultCameraBoomLocation;
	FRotator DefaultCameraBoomRotation;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash|Animation")
	UAnimMontage* DashMontage;

	FHitResult HitResult;

	UPROPERTY(BlueprintReadOnly)
	int CollectedCollectables = 0;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float MaximumInteractionDistance = 250.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float InteractionRadius = 30.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	bool bToggleDebugVisualiser = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FVector SweepStartOffset = FVector(0.f, 0.f, 50.f); // Example: Start from head height
	//UPROPERTY()
	//AInspectInteractables* InspectInteract;
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
	float InitialArmLength = 300.f;
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

	FRotator TargetBoomRotation;



	float StartingFOV;
	float TargetFOV;
	float FOVResetTimer = 0.f;
	float FOVResetDuration = 0.4f; // Adjust speed
	bool bIsResettingFOV = false;


	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* BeamMesh;

	// Beam growing parameters
	UPROPERTY(EditAnywhere, Category = "Beam")
	float BeamMaxLength = 100.f; // max length beam grows to

	float BeamGrowthSpeed = 10.f; // units per second

private:
	float CurrentBeamLength = 0.f;

	// To identify the chest socket name
	UPROPERTY(EditDefaultsOnly, Category = "Beam")
	FName ChestSocketName = TEXT("ChestSocket");



	bool bIsZoomedIn = false;
	float DefaultFOV = 90.f;
	float ZoomedFOV = 60.f;
	float CameraZoomSpeed = 10.f;

	// Camera Boom (SpringArm) offsets
	FVector NormalArmOffset = FVector(0.f, 50.f, 60.f);   // normal shoulder camera
	FVector ZoomedArmOffset = FVector(0.f, 20.f, 75.f);    // closer over-the-shoulder

	float ArmLength = 300.f;
	float ZoomedArmLength = 150.f;
};