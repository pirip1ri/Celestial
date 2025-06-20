// Fill out your copyright notice in the Description page of Project Settings.


#include "InspectInteractables.h"
#include <Kismet/GameplayStatics.h>
#include "CelestialPlayerController.h"
#include "InspectInteractable.h"
#include "PlayerCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "MenuBaseWidget.h"
#include "PauseMenuWidget.h"

#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Texture2D.h"

AInspectInteractables::AInspectInteractables()
{
    PrimaryActorTick.bCanEverTick = true;

    InspectCameraAnchor = CreateDefaultSubobject<USceneComponent>(TEXT("InspectCameraAnchor"));
    InspectCameraAnchor->SetupAttachment(RootComponent);
    InspectCameraAnchor->SetRelativeLocation(FVector(0.f, 0.f, 80.f));
    InspectCameraAnchor->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
}

void AInspectInteractables::InteractAbility()
{
    Super::InteractAbility();

    TargetPlayer = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!TargetPlayer || !InspectCameraAnchor)
        return;

    bIsInspecting = true;

    if (ACelestialPlayerController* Controller = Cast<ACelestialPlayerController>(TargetPlayer->GetController()))
    {
        Controller->CurrentInspectInteractable = this;
    }

    if (WidgetClass)
    {
        Widget = CreateWidget<UMenuBaseWidget>(GetWorld(), WidgetClass);
        if (Widget)
        {
            Widget->AddToViewport();

            UTexture2D* MyTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/CelestialPrototypes/Textures/BadBlueprintSprite.BadBlueprintSprite"));
            if (UPauseMenuWidget* PauseMenu = Cast<UPauseMenuWidget>(Widget))
            {
                constexpr float ShowMenuDelay = 1.5f;

                GetWorld()->GetTimerManager().SetTimer(ContextualMenuDelayHandle, FTimerDelegate::CreateWeakLambda(this, [this, PauseMenu, MyTexture]()
                {
                    if (PauseMenu && bIsInspecting)
                    {
                        PauseMenu->ToggleContextualMenu(true, FText::FromString(TEXT("Inspecting Item")), MyTexture, false, true);
                    }
                }), ShowMenuDelay, false);
            }
        }
    }
}


void AInspectInteractables::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bIsInspecting || !TargetPlayer || !InspectCameraAnchor)
        return;

    USpringArmComponent* CameraBoom = TargetPlayer->CameraBoom;
    if (!CameraBoom) return;

    const FVector TargetLocation = InspectCameraAnchor->GetComponentLocation();
    const FRotator TargetRotation = InspectCameraAnchor->GetComponentRotation();

    const FVector RelativeLocation = TargetPlayer->GetActorTransform().InverseTransformPosition(TargetLocation);
    const FRotator RelativeRotation = TargetRotation - TargetPlayer->GetActorRotation();

    CameraBoom->SocketOffset = FMath::VInterpTo(CameraBoom->SocketOffset, RelativeLocation, DeltaTime, 5.f);
    CameraBoom->SetRelativeRotation(FMath::RInterpTo(CameraBoom->GetRelativeRotation(), RelativeRotation, DeltaTime, 5.f));
    CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, InspectArmLength, DeltaTime, 5.f);
}

void AInspectInteractables::EndInspection()
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
        PC->SetInputMode(FInputModeGameOnly());
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
        }

        Widget = nullptr;
    }
}

APlayerCharacter* AInspectInteractables::GetTargetPlayer()
{
    return TargetPlayer;

}




