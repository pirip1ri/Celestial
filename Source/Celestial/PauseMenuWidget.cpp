// Fill out your copyright notice in the Description page of Project Settings.

#include "PauseMenuWidget.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Image.h"
#include "CelestialPlayerController.h"
 #include "InspectInteractables.h"

void UPauseMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ResumeButton)
    {
        ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnResumeClicked);
    }

    if (MainMenuButton)
    {
        MainMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnMainMenuClicked);
    }
    if (ExitInspectButton)
    {
        ExitInspectButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnExitInspectClicked);
    }
}



void UPauseMenuWidget::TogglePauseMenu()
{
    bIsVisible = !bIsVisible;

    if (bIsVisible)
    {
        ShowMenu(true, true);
        Text->SetText(FText::FromString("Paused"));
    }
    else
    {
        HideMenu(true);
    }
}

void UPauseMenuWidget::OnResumeClicked()
{
    TogglePauseMenu();
}

void UPauseMenuWidget::OnMainMenuClicked()
{
    UGameplayStatics::SetGamePaused(GetWorld(), false); 
    // Make sure this level exists//
    //UGameplayStatics::OpenLevel(this, FName("MainMenu"));
}
void UPauseMenuWidget::ToggleContextualMenu(bool bShow, const FText& Message, UTexture2D* OptionalImage, bool bShowPauseButtons, bool bIsInspectionMenu)
{
    bIsVisible = bShow;

    if (!bShow)
    {
        HideMenu(false);
       // ClearContextualMenuContent();
        return;
    }

    // Instantly show the menu
    ShowMenu(true, true, 0.0f);

    if (Text)
    {
        Text->SetText(Message);
    }

    if (InspectImage)
    {
        if (OptionalImage)
        {
            InspectImage->SetBrushFromTexture(OptionalImage);
            InspectImage->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            InspectImage->SetVisibility(ESlateVisibility::Collapsed);
        }
    }

    if (bIsInspectionMenu)
    {
        if (ResumeButton) ResumeButton->SetVisibility(ESlateVisibility::Collapsed);
        if (MainMenuButton) MainMenuButton->SetVisibility(ESlateVisibility::Collapsed);
        if (ExitInspectButton) ExitInspectButton->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        if (ResumeButton) ResumeButton->SetVisibility(bShowPauseButtons ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
        if (MainMenuButton) MainMenuButton->SetVisibility(bShowPauseButtons ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
        if (ExitInspectButton) ExitInspectButton->SetVisibility(ESlateVisibility::Collapsed);
    }


}

void UPauseMenuWidget::OnExitInspectClicked()
{
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        if (ACelestialPlayerController* CelestialPC = Cast<ACelestialPlayerController>(PC))
        {
            if (CelestialPC->CurrentInspectInteractable)
            {
                CelestialPC->CurrentInspectInteractable->EndInspection();
            }
        }
    }
}


// Keeps simple Pause behavior
void UPauseMenuWidget::ToggleOtherMenu()
{
  //  ToggleContextualMenu(!bIsVisible, FText::FromString("Paused"), nullptr, true);
}

void UPauseMenuWidget::OnQuitClicked()
{
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, false);
}

void UPauseMenuWidget::OnMenuOpened()
{
    Super::OnMenuOpened();

    bIsVisible = true;
}

void UPauseMenuWidget::OnMenuClosed()
{
    Super::OnMenuClosed();
    bIsVisible = false;
}
