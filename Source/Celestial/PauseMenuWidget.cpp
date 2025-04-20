// Fill out your copyright notice in the Description page of Project Settings.

#include "PauseMenuWidget.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/KismetSystemLibrary.h"

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
