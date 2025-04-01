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
UCLASS()
class CELESTIAL_API ACelestialPlayerController : public APlayerController
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;
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
    UInputAction* CrouchAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* JumpAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* InteractAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* PauseAction;

    // Input Action Functions
    void MoveForward(const FInputActionValue& Value);
    void MoveRight(const FInputActionValue& Value);
    void LookUp(const FInputActionValue& Value);
    void Turn(const FInputActionValue& Value);
    void Sprint(const FInputActionValue& Value);
    void Crouch(const FInputActionValue& Value);
    void JumpFunction();
    void JumpStopFunction();
    void InteractWithObject();

    // Pause the game
    void TogglePause();
    // Helper function to set input mode
    void SetInputModeForPause(bool bIsPaused);
};
