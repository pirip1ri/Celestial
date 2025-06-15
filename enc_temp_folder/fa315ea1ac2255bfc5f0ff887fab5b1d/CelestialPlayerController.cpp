// Fill out your copyright notice in the Description page of Project Settings.


#include "CelestialPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "PauseMenuWidget.h"
#include "Rotatereflecter.h"
#include "CelestialTutorialPromptManager.h"
#include "InspectInteractable.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InteractableDoor.h"
#include "InteractableInterface.h"

void ACelestialPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Add the IMC (Input Mapping Context) if available
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(DefaultMappingContext, 0);  // Add the default input mapping context
    }

    // Get reference to PromptManager for tutorial prompt handling
    ACelestialTutorialPromptManager* PromptManager = Cast<ACelestialTutorialPromptManager>(
        UGameplayStatics::GetActorOfClass(GetWorld(), ACelestialTutorialPromptManager::StaticClass())
    );

    // Bind tutorial actions if the PromptManager exists
    if (PromptManager && InputComponent)
    {
        if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
        {
            const TArray<FPromptData>& Prompts = PromptManager->GetPromptList();
            for (const FPromptData& Prompt : Prompts)
            {
                if (Prompt.InputAction)
                {
                    // Bind the input actions for tutorial prompts
                    EnhancedInput->BindAction(Prompt.InputAction, ETriggerEvent::Triggered, PromptManager, &ACelestialTutorialPromptManager::HandlePromptInput);
                }
            }
        }
    }

    // Start the tutorial when BeginPlay() is called (Optional)
    StartTutorial();
}

void ACelestialPlayerController::HandleTutorialNextStep(const FInputActionInstance& Instance)
{
    if (bTutorialModeActive)
    {
        // Proceed to the next tutorial step
        AdvanceTutorialStep();
    }
}

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

void ACelestialPlayerController::StartTutorial()
{
    bTutorialModeActive = true;  // Enable tutorial mode
    TutorialStep = 0;           // Reset tutorial step counter
    AdvanceTutorialStep();      // Start with the first tutorial step
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
           EnhancedInput->BindAction(InteractAction, ETriggerEvent::Completed, this, &ACelestialPlayerController::EndInteraction);
          
            
        
            // Bind the pause action
            EnhancedInput->BindAction(PauseAction, ETriggerEvent::Started, this, &ACelestialPlayerController::TogglePause);
            EnhancedInput->BindAction(InspectAction, ETriggerEvent::Started, this, &ACelestialPlayerController::HandleInspectInteraction);
          
   

            // Bind Crouch
            EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACelestialPlayerController::JumpFunction);
            EnhancedInput->BindAction(BeemAction, ETriggerEvent::Triggered, this, &ACelestialPlayerController::OnBeamPressed);
            EnhancedInput->BindAction(BeemAction, ETriggerEvent::Completed, this, &ACelestialPlayerController::OnBeamReleased);
            EnhancedInput->BindAction(ZoomAction, ETriggerEvent::Started, this, &ACelestialPlayerController::ToggleCharacterZoom);
            EnhancedInput->BindAction(ZoomAction, ETriggerEvent::Completed, this, &ACelestialPlayerController::ToggleCharacterZoom);

            EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACelestialPlayerController::JumpStopFunction);
            // for mouseinteraction input
            EnhancedInput->BindAction(MouseTurnAction, ETriggerEvent::Triggered, this, &ACelestialPlayerController::HandleReflectors);
        
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
        if (bTutorialModeActive)
        {
            return;
        }
        float ForwardValue = Value.Get<float>(); // Get the input value as a float
        PlayerCharacter->MoveForward(ForwardValue);
    }
}

void ACelestialPlayerController::MoveRight(const FInputActionValue& Value)
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
    {
        if (bTutorialModeActive)
        {
            return;
        }
        float RightValue = Value.Get<float>();
        PlayerCharacter->MoveRight(RightValue);
    }
}

void ACelestialPlayerController::LookUp(const FInputActionValue& Value)
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
    {
        if (bTutorialModeActive)
        {
            return;
        }
        float LookUpValue = Value.Get<float>();
        PlayerCharacter->LookUp(LookUpValue);
    }
}

void ACelestialPlayerController::Turn(const FInputActionValue& Value)
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
    {
        if (bTutorialModeActive)
        {
            return;
        }
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
        if (bTutorialModeActive)
        {
            return;
        }
        PlayerCharacter->PlayDashMontage_Implementation();
    }
}

void ACelestialPlayerController::Crouch(const FInputActionValue& Value)
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
    {
        if (bTutorialModeActive)
        {
            return;
        }
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
    if (bTutorialModeActive)
    {
        return;
    }
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
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    if (!PlayerCharacter) return;

    AActor* HitActor = PlayerCharacter->HitResult.GetActor();
    if (!HitActor) return;

    if (HandleReflectorInteraction(HitActor, PlayerCharacter)) return;

    ResetReflectorState(PlayerCharacter);



    if (HandleDoorInteraction(HitActor)) return;

    // Fallback reset
    Door = nullptr;
  
}

bool ACelestialPlayerController::HandleReflectorInteraction(AActor* Hit, APlayerCharacter* PlayerCharacter)
{
    if (ARotatereflecter* HitReflector = Cast<ARotatereflecter>(Hit))
    {
        if (IInteractableInterface* Interactable = Cast<IInteractableInterface>(HitReflector))
        {
            if (ActiveReflector != HitReflector &&!CurrentInspectInteractable)
            {
                SetActiveReflector(HitReflector);
                PlayerCharacter->CameraBoom->bUsePawnControlRotation = false;

                
            }

            return true; // handled
        }
    }

  
    return false; // not a reflector
}


void ACelestialPlayerController::ResetReflectorState(APlayerCharacter* PlayerCharacter)
{
    EndInteraction();
    SetActiveReflector(nullptr);
    PlayerCharacter->CameraBoom->bUsePawnControlRotation = true;
}

// Inside ACelestialPlayerController
// In your PlayerController.cpp

void ACelestialPlayerController::HandleInspectInteraction()
{
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    PlayerCharacter->TryInteract();
    
}



bool ACelestialPlayerController::HandleDoorInteraction(AActor* HitActor)
{
    if (AInteractableDoor* HitDoor = Cast<AInteractableDoor>(HitActor))
    {
        Door = HitDoor;
        CurrentInspectInteractable = nullptr;

        if (!ActiveReflector)
        {
            Door->InteractAbility_Implementation();
        }

        return true; // handled
    }

    return false;
}


            
        
bool ACelestialPlayerController::IsInteracting() const
{
    return bIsInteracting;
}
void ACelestialPlayerController::EndInteraction()
{
    bIsInteracting = false;
}
void ACelestialPlayerController::OnBeamPressed()
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
    {
        PlayerCharacter->StartBeam();
    }
}

void ACelestialPlayerController::OnBeamReleased()
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
    {
        PlayerCharacter->StopBeam();
    }
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
            true,
            false
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
void ACelestialPlayerController:: HandleReflectors(const FInputActionValue& Value)
{
    const FVector2D AxisValue = Value.Get<FVector2D>(); 

    
    TurnReflector(AxisValue.X);       
    LookUpReflector(AxisValue.Y);
}
void ACelestialPlayerController::ToggleCharacterZoom()
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
    {
        PlayerCharacter->ToggleZoom();
    }
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

void ACelestialPlayerController::ApplyCameraZoomAndTilt(bool bZoomOut)
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
    {
        TargetArmLength = bZoomOut ? 700.0f : 300.0f;
        TargetBoomRotation = bZoomOut ? FRotator(-25.0f, 0.0f, 0.0f) : FRotator::ZeroRotator;
        TargetSocketOffset = bZoomOut ? FVector(0.0f, 0.0f, 100.0f) : FVector::ZeroVector;
        bShouldZoom = true;
    }
}



void ACelestialPlayerController::SetActiveReflector(ARotatereflecter* Reflector)
{
    ActiveReflector = Reflector;
    ApplyCameraZoomAndTilt(Reflector != nullptr);
}


void ACelestialPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (bShouldZoom)
    {
        if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
        {
            USpringArmComponent* CameraBoom = PlayerCharacter->CameraBoom;
            if (!CameraBoom) return;

            CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, TargetArmLength, DeltaSeconds, CameraZoomInterpSpeed);
            CameraBoom->SetRelativeRotation(FMath::RInterpTo(CameraBoom->GetRelativeRotation(), TargetBoomRotation, DeltaSeconds, CameraZoomInterpSpeed));
            CameraBoom->SocketOffset = FMath::VInterpTo(CameraBoom->SocketOffset, TargetSocketOffset, DeltaSeconds, CameraZoomInterpSpeed);

            // Stop updating if we're close enough
            if (FMath::IsNearlyEqual(CameraBoom->TargetArmLength, TargetArmLength, 0.5f) &&
                CameraBoom->GetRelativeRotation().Equals(TargetBoomRotation, 0.5f) &&
                CameraBoom->SocketOffset.Equals(TargetSocketOffset, 1.0f))
            {
                bShouldZoom = false;
            }
        }
    }
}
