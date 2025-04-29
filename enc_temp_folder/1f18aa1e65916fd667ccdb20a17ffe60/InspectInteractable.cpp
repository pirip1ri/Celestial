#include "InspectInteractable.h"
#include "PlayerCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/SpringArmComponent.h" 
#include "MenuBaseWidget.h"
#include "PauseMenuWidget.h"
#include "CelestialPlayerController.h"

AInspectInteractable::AInspectInteractable()
{
    PrimaryActorTick.bCanEverTick = true;

    InspectCameraAnchor = CreateDefaultSubobject<USceneComponent>("Anchor");
    InspectCameraAnchor->SetRelativeLocation(FVector(0.f, 0.f, 80.f));
    InspectCameraAnchor->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
}
void AInspectInteractable::InteractAbility_Implementation()
{
    Super::InteractAbility_Implementation();

    TargetPlayer = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!TargetPlayer || !InspectCameraAnchor) return;

    bIsInspecting = true;

    if (ACelestialPlayerController* Controller = Cast<ACelestialPlayerController>(TargetPlayer->GetController()))
    {
        Controller->CurrentInspectInteractable = this;
    }

    
    // We only save camera once at BeginPlay.

    if (WidgetClass)
    {
        Widget = CreateWidget<UMenuBaseWidget>(GetWorld(), WidgetClass);
        if (Widget)
        {
            Widget->AddToViewport();

            UTexture2D* MyTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/CelestialPrototypes/Textures/BadBlueprintSprite.BadBlueprintSprite'"));

            if (UPauseMenuWidget* PauseMenu = Cast<UPauseMenuWidget>(Widget))
            {
                float ShowMenuDelay = 1.5f; // Match zoom timing

                FTimerHandle DelayHandle;
                GetWorld()->GetTimerManager().SetTimer(ContextualMenuDelayHandle, FTimerDelegate::CreateWeakLambda(this, [this, PauseMenu, MyTexture]()
                {
                    if (PauseMenu && bIsInspecting)
                    {
                        PauseMenu->ToggleContextualMenu(true, FText::FromString("Inspecting Item"), MyTexture, false, true);
                    }
                }), ShowMenuDelay, false);
            }
        }
    }
}

void AInspectInteractable::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);



    

    if (bIsInspecting && TargetPlayer && InspectCameraAnchor)
    {
        FVector TargetLocation = InspectCameraAnchor->GetComponentLocation();
        FRotator TargetRotation = InspectCameraAnchor->GetComponentRotation();

        FVector CurrentLocation = TargetPlayer->CameraBoom->GetComponentLocation();
        FRotator CurrentRotation = TargetPlayer->CameraBoom->GetComponentRotation();
        float CurrentArmLength = TargetPlayer->CameraBoom->TargetArmLength;

        FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, 5.0f);
        FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 5.0f);
        float NewArmLength = FMath::FInterpTo(CurrentArmLength, InspectArmLength, DeltaTime, 5.0f);

        TargetPlayer->CameraBoom->SetWorldLocation(NewLocation);
        TargetPlayer->CameraBoom->SetWorldRotation(NewRotation);
        TargetPlayer->CameraBoom->TargetArmLength = NewArmLength;
    }
}


void AInspectInteractable::EndInspection()
{
    if (!bIsInspecting) return;

    bIsInspecting = false;

    if (TargetPlayer)
    {
        if (ACelestialPlayerController* Controller = Cast<ACelestialPlayerController>(TargetPlayer->GetController()))
        {
            Controller->CurrentInspectInteractable = nullptr;
        }

        TargetPlayer->ResetCameraAfterInspect();
    }

    TargetPlayer = nullptr;

    UGameplayStatics::SetGamePaused(GetWorld(), false);

    if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = false;
    }

    if (Widget)
    {
        if (UPauseMenuWidget* PauseMenu = Cast<UPauseMenuWidget>(Widget))
        {
            PauseMenu->ToggleContextualMenu(false, FText::GetEmpty(), nullptr, false, false);
        }

        if (Widget->IsInViewport())
        {
            Widget->RemoveFromParent();
            Widget = nullptr;
        }
    }
}
