// Fill out your copyright notice in the Description page of Project Settings.


#include "CelestialPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"

void ACelestialPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Add the IMC
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }
}

void ACelestialPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // Enhanced Input Component
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
    {
        // Bind Movement
        EnhancedInput->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ACelestialPlayerController::MoveForward);
        EnhancedInput->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ACelestialPlayerController::MoveRight);

        // Bind Looking
        EnhancedInput->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &ACelestialPlayerController::LookUp);
        EnhancedInput->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ACelestialPlayerController::Turn);

        // Bind Sprint
        EnhancedInput->BindAction(SprintAction, ETriggerEvent::Started, this, &ACelestialPlayerController::Sprint);
        EnhancedInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &ACelestialPlayerController::Sprint);

        // Bind Dash
        EnhancedInput->BindAction(DashAction, ETriggerEvent::Started, this, &ACelestialPlayerController::Dash);

        // Bind Crouch
        EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Started, this, &ACelestialPlayerController::Crouch);
        EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ACelestialPlayerController::Crouch);

        // Bind Interact
        EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, this, &ACelestialPlayerController::InteractWithObject);

        // Bind the pause action
        EnhancedInput->BindAction(PauseAction, ETriggerEvent::Started, this, &ACelestialPlayerController::TogglePause);

        // Bind Crouch
        EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACelestialPlayerController::JumpFunction);
        EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACelestialPlayerController::JumpStopFunction);
    }
}

void ACelestialPlayerController::MoveForward(const FInputActionValue& Value)
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
    {
        float ForwardValue = Value.Get<float>(); // Get the input value as a float
        PlayerCharacter->MoveForward(ForwardValue);
    }
}

void ACelestialPlayerController::MoveRight(const FInputActionValue& Value)
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
    {
        float RightValue = Value.Get<float>();
        PlayerCharacter->MoveRight(RightValue);
    }
}

void ACelestialPlayerController::LookUp(const FInputActionValue& Value)
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
    {
        float LookUpValue = Value.Get<float>();
        PlayerCharacter->LookUp(LookUpValue);
    }
}

void ACelestialPlayerController::Turn(const FInputActionValue& Value)
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
    {
        float TurnValue = Value.Get<float>();
        PlayerCharacter->Turn(TurnValue);
    }
}

void ACelestialPlayerController::Sprint(const FInputActionValue& Value)
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
    {
        if (Value.Get<bool>())
        {
            PlayerCharacter->StartSprint();
        }
        else
        {
            PlayerCharacter->StopSprint();
        }
    }
}

void ACelestialPlayerController::Dash(const FInputActionValue& Value)
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
    {
        PlayerCharacter->PlayDashMontage();
    }
}

void ACelestialPlayerController::Crouch(const FInputActionValue& Value)
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
    {
        if (Value.Get<bool>())
        {
            PlayerCharacter->StartCrouch();
        }
        else
        {
            PlayerCharacter->StopCrouch();
        }
    }
}

void ACelestialPlayerController::JumpFunction()
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
    {
        PlayerCharacter->Jump();
    }
}

void ACelestialPlayerController::JumpStopFunction()
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
    {
        PlayerCharacter->StopJumping();
    }
}

void ACelestialPlayerController::InteractWithObject()
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
    {
        PlayerCharacter->Interact();
    }
}

void ACelestialPlayerController::TogglePause()
{
    UE_LOG(LogTemp, Display, TEXT("TogglePause"));

    /*
    // Get the current game mode
    ADescentGameModeBase* GameMode = Cast<ADescentGameModeBase>(UGameplayStatics::GetGameMode(this));
    if (!GameMode)
    {
        UE_LOG(LogTemp, Warning, TEXT("TogglePause: Could not find DescentGameModeBase"));
        return;
    }

    // Use the GameMode's functionality to toggle the pause menu
    GameMode->ChangeGameState(EDescentGameState::Paused);
    */
}

void ACelestialPlayerController::SetInputModeForPause(bool bIsPaused)
{
    /*
    if (bIsPaused)
    {
        // Set input mode for UI and show mouse cursor
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
    }
    else
    {
        // Set input mode for gameplay and hide mouse cursor
        SetInputMode(FInputModeGameOnly());
        bShowMouseCursor = false;
    }
    */
}
