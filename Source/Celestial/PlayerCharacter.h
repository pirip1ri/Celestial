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
class ULightBeamSpawnerComponent;

UCLASS()
class CELESTIAL_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere)
	ULightBeamSpawnerComponent* LightBeamSpawner;

public:
	// Sets default values for this character's properties
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
<<<<<<< Updated upstream
	void CastLight();
	void CastLightEnd();
=======

	FVector DefaultCameraLocation;
	FRotator DefaultCameraRotation;
	float DefaultCameraBoomLength;
	//for pick ups
	void AddKey(FName Key);

	void CastLight();

	void CastLightEnd();

	bool HasKey(FName KeyID) const;

	void TryInteract();

	void ResetCameraAfterInspect();

	void SaveCameraDefaults();
	void ToggleZoom();
	void UpdateZoom(float DeltaTime);
	FVector DefaultCameraBoomLocation;
	FRotator DefaultCameraBoomRotation;


	FHitResult HitResult;
>>>>>>> Stashed changes

	UPROPERTY(BlueprintReadOnly)
	int CurrentLevelsCollectedCollectables = 0;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightBeam")
	FName LightBeamSocketName = "spine_05"; // Default socket name
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsFiringBeam = false;

	AInteractable* LastInteractable;
private:
	float DeadZoneThreshold = 0.1f;

};
