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
        USpringArmComponent* CameraBoom = TargetPlayer->CameraBoom;
        if (!CameraBoom) return;

        FVector TargetLocation = InspectCameraAnchor->GetComponentLocation();
        FRotator TargetRotation = InspectCameraAnchor->GetComponentRotation();

        // Compute local transform relative to the player
        FVector RelativeTargetLocation = TargetPlayer->GetActorTransform().InverseTransformPosition(TargetLocation);
        FRotator RelativeTargetRotation = TargetRotation - TargetPlayer->GetActorRotation();

        // Interpolate relative offset and rotation
        FVector NewSocketOffset = FMath::VInterpTo(CameraBoom->SocketOffset, RelativeTargetLocation, DeltaTime, 5.0f);
        FRotator NewRelativeRotation = FMath::RInterpTo(CameraBoom->GetRelativeRotation(), RelativeTargetRotation, DeltaTime, 5.0f);
        float NewArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, InspectArmLength, DeltaTime, 5.0f);

     
        CameraBoom->SocketOffset = NewSocketOffset;
        CameraBoom->SetRelativeRotation(NewRelativeRotation);
        CameraBoom->TargetArmLength = NewArmLength;

        // DO NOT set world location/rotation manually — keep bDoCollisionTest enabled
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
