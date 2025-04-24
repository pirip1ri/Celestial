// Fill out your copyright notice in the Description page of Project Settings.

#include "PauseMenuWidget.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Image.h"

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


void UPauseMenuWidget::ToggleContextualMenu(bool bShow, const FText& Message, UTexture2D* OptionalImage, bool bShowPauseButtons)
{
    bIsVisible = bShow;

    if (bShow)
    {
        FTimerHandle TempHandle;
        GetWorld()->GetTimerManager().SetTimer(TempHandle, FTimerDelegate::CreateLambda(
            [this, Message, OptionalImage, bShowPauseButtons]()
        {
            ShowMenu(true, true); // Show now (no delay inside)

            if (Text)
            {
                Text->SetText(Message);
            }

            if (InspectImage && OptionalImage)
            {
                InspectImage->SetBrushFromTexture(OptionalImage);
                InspectImage->SetVisibility(ESlateVisibility::Visible);
            }
            else if (InspectImage)
            {
                InspectImage->SetVisibility(ESlateVisibility::Collapsed);
            }

            if (ResumeButton)
            {
                ResumeButton->SetVisibility(bShowPauseButtons ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
            }

            if (MainMenuButton)
            {
                MainMenuButton->SetVisibility(bShowPauseButtons ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
            }

        }), 2.0f, false); // Match your intended delay
    }
    else
    {
        HideMenu(true);
    }
}

// Keeps simple Pause behavior
void UPauseMenuWidget::ToggleOtherMenu()
{
    ToggleContextualMenu(!bIsVisible, FText::FromString("Paused"), nullptr, true);
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
