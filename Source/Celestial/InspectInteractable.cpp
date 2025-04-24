// Fill out your copyright notice in the Description page of Project Settings.


#include "InspectInteractable.h"
#include "PlayerCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/SpringArmComponent.h" 
#include "MenuBaseWidget.h"
#include "PauseMenuWidget.h"
AInspectInteractable::AInspectInteractable()
{
    InspectCameraAnchor = CreateDefaultSubobject<USceneComponent>("Anchor");
   
    InspectCameraAnchor->SetRelativeLocation(FVector(0.f, 0.f, 80.f)); // Move it upward
    InspectCameraAnchor->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
}
void AInspectInteractable::InteractAbility_Implementation()
{
    Super::InteractAbility_Implementation();

    APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (Player && InspectCameraAnchor)
    {

        FVector TargetLocation = InspectCameraAnchor->GetComponentLocation();
        FRotator TargetRotation = InspectCameraAnchor->GetComponentRotation();


        FVector CurrentLocation = Player->CameraBoom->GetComponentLocation();
        FRotator CurrentRotation = Player->CameraBoom->GetComponentRotation();


        FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, GetWorld()->GetDeltaSeconds(), 5.0f);
        
        FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 5.0f);
       

        Player->CameraBoom->SetWorldLocation(NewLocation);
        Player->CameraBoom->SetWorldRotation(NewRotation);

        // Smooth zoom in
         Widget = CreateWidget<UMenuBaseWidget>(GetWorld(), WidgetClass);
        Player->CameraBoom->TargetArmLength = FMath::FInterpTo(Player->CameraBoom->TargetArmLength, 100.f, GetWorld()->GetDeltaSeconds(), 5.0f);
     
        if (UMenuBaseWidget* MenuWidget = Cast<UMenuBaseWidget>(Widget))
        {
            
            MenuWidget->AddToViewport();

            UTexture2D* MyTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/CelestialPrototypes/Textures/BadBlueprintSprite.BadBlueprintSprite'"));
            if (UPauseMenuWidget* PauseMenu = Cast<UPauseMenuWidget>(MenuWidget))
            {
                PauseMenu->ToggleContextualMenu(true, FText::FromString("Test"), MyTexture, false);
               
            }
        }

    }
}

void AInspectInteractable::EndInspection()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player)
	{
		Player->ResetCameraAfterInspect();
       /* if (UPauseMenuWidget* PauseMenu = Cast<UPauseMenuWidget>(MenuWidget))
        {
            PauseMenu->ToggleContextualMenu(false, FText::FromString(""), nullptr, false)

        }*/
        
	}
}