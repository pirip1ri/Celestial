// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "CelestialPlayerController.generated.h"

/**
 *
 */
class AInspectInteractable;
struct FInputBindingHandle;
class ARotatereflecter;
class UPauseMenuWidget;

UCLASS()
class CELESTIAL_API ACelestialPlayerController : public APlayerController
{
    GENERATED_BODY()

protected:
   
    virtual void BeginPlay() override;
    void HandleTutorialNextStep(const FInputActionInstance& Instance);
    void AdvanceTutorialStep();
    void StartTutorial();
    virtual void SetupInputComponent() override;
    bool bIsInteracting;
private:
    // Input Mapping Context and Input Actions
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* MoveForwardAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* MoveRightAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* LookUpAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* TurnAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* SprintAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* DashAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* CrouchAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* JumpAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* InteractAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* PauseAction;
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* MouseTurnAction;
     UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* ExitInspectionAction; 
    
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* BeemAction;  
    
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* ZoomAction;

    UPROPERTY()
    UPauseMenuWidget* PauseWidget;
    
   UPROPERTY(EditDefaultsOnly)
   TSubclassOf< UPauseMenuWidget> PauseWidgetclass;
   UPROPERTY()
   ARotatereflecter* ActiveReflector;
   
   UPROPERTY(EditDefaultsOnly)
  TSubclassOf<ARotatereflecter> ActiveReflectorClass;
    void MoveForward(const FInputActionValue& Value);
    void MoveRight(const FInputActionValue& Value);
    void LookUp(const FInputActionValue& Value);
    void Turn(const FInputActionValue& Value);
    void Sprint(const FInputActionValue& Value);
    void Dash(const FInputActionValue& Value);
    void Crouch(const FInputActionValue& Value);
    void HandleReflectors(const FInputActionValue& Value);
    void ToggleCharacterZoom();
    void JumpFunction();
    void JumpStopFunction();
    void InteractWithObject();
    bool IsInteracting() const;
    void  TurnReflector(float Value);
    void LookUpReflector(float Value);
    void EndInteraction();
    void OnBeamPressed();
    void OnBeamReleased();
  
  
    void TogglePause();
    void ToggleInspect();
    // Helper function to set input mode
    void SetInputModeForPause(bool bIsPaused);
  
    UPROPERTY()
    UInputAction* TutorialNextAction;
    
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AInspectInteractable> InspectClass;

    
    float  RotationSensitivity = 50.0f;
    bool bTutorialModeActive = false;
public:
    int32 TutorialStep = 0;
    UPROPERTY()
    AInspectInteractable* CurrentInspectInteractable;
   
    void ApplyCameraZoomAndTilt(bool bZoomOut);

    void SetActiveReflector(class ARotatereflecter* Reflector);
  //virtual  void Tick(float DeltaTime)override;
   UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
   bool bTriggerWhenPaused = true;
 
};
