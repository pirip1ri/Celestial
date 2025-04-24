// Fill out your copyright notice in the Description page of Project Settings.


#include "CelestialPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "PauseMenuWidget.h"
#include "Rotatereflecter.h"
#include "CelestialTutorialPromptManager.h"

void ACelestialPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Add the IMC
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }

    

    // Get reference to PromptManager
    ACelestialTutorialPromptManager* PromptManager = Cast<ACelestialTutorialPromptManager>(
        UGameplayStatics::GetActorOfClass(GetWorld(), ACelestialTutorialPromptManager::StaticClass())
    );

    if (PromptManager && InputComponent)
    {
        if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
        {
            const TArray<FPromptData>& Prompts = PromptManager->GetPromptList();
            for (const FPromptData& Prompt : Prompts)
            {
                if (Prompt.InputAction)
                {
                    EnhancedInput->BindAction(Prompt.InputAction, ETriggerEvent::Started, PromptManager, &ACelestialTutorialPromptManager::AdvancePromptFromController);
                }
            }
        }
    }
}


void ACelestialPlayerController::HandleTutorialNextStep(const FInputActionInstance& Instance)
{
    if (bTutorialModeActive)
    {
        UE_LOG(LogTemp, Log, TEXT("Tutorial step triggered!"));

        // Your tutorial logic here ( progress to next step)
        AdvanceTutorialStep();
    }
}

int32 TutorialStep = 0;
void ACelestialPlayerController::AdvanceTutorialStep()
{
    TutorialStep++;

    switch (TutorialStep)
    {
    case 1:
        UE_LOG(LogTemp, Log, TEXT("Step 1: Move forward."));
        break;
    case 2:
        UE_LOG(LogTemp, Log, TEXT("Step 2: Jump."));
        break;
    case 3:
        UE_LOG(LogTemp, Log, TEXT("Tutorial complete."));
        bTutorialModeActive = false;
        break;
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
        EnhancedInput->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ACelestialPlayerController::InteractWithObject);
        //EnhancedInput->BindAction(InteractAction, ETriggerEvent::Completed, this, &ACelestialPlayerController::InteractWithObject);

        // Bind the pause action
        EnhancedInput->BindAction(PauseAction, ETriggerEvent::Started, this, &ACelestialPlayerController::TogglePause);
   

        // Bind Crouch
        EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACelestialPlayerController::JumpFunction);
        EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACelestialPlayerController::JumpStopFunction);
        // for mouseinteraction input
        EnhancedInput->BindAction(MouseTurnAction, ETriggerEvent::Triggered, this, &ACelestialPlayerController::Test);
        
        if (TutorialNextAction)
        {
            EnhancedInput->BindAction(TutorialNextAction, ETriggerEvent::Triggered, this, &ACelestialPlayerController::HandleTutorialNextStep);
        }
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
        if (ActiveReflector)
        {
            if (ActiveReflector->IsInteracting())
            {
                ActiveReflector->EndInteraction();
                SetActiveReflector(nullptr); // optional
            }
            else
            {
                PlayerCharacter->Interact();
            }
        }
        else
        {
            PlayerCharacter->Interact();
        }
    }
}
bool ARotatereflecter::IsInteracting() const
{
    return bIsInteracting;
}
void ARotatereflecter::EndInteraction()
{
    bIsInteracting = false;
}
void ACelestialPlayerController::TogglePause()
{
    if (!PauseWidget)
    {
        PauseWidget = CreateWidget<UPauseMenuWidget>(this, PauseWidgetclass);
    }

    if (PauseWidget)
    {
        PauseWidget->ToggleContextualMenu(
            true,
            FText::FromString("Paused"),
            nullptr,               
            true                  
        );
    }




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
// mouse input x/y if needed
void ACelestialPlayerController::Test(const FInputActionValue& Value)
{
    const FVector2D AxisValue = Value.Get<FVector2D>(); 

    // Apply to reflector
    TurnReflector(AxisValue.X);       
    LookUpReflector(AxisValue.Y);
}
void ACelestialPlayerController::LookUpReflector(float Value)
{
    if (ActiveReflector && FMath::Abs(Value) > KINDA_SMALL_NUMBER)
    {
        float DeltaTime = GetWorld()->GetDeltaSeconds();
        ActiveReflector->AddPitchInput(Value * RotationSensitivity * DeltaTime);
    }
}

void ACelestialPlayerController::TurnReflector(float Value)
{
    if (ActiveReflector && FMath::Abs(Value) > KINDA_SMALL_NUMBER)
    {
        float DeltaTime = GetWorld()->GetDeltaSeconds();
        ActiveReflector->AddYawInput(Value * RotationSensitivity * DeltaTime);
    }
}


void ACelestialPlayerController::SetActiveReflector(ARotatereflecter* Reflector)
{
    ActiveReflector = Reflector;
}