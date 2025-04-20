// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuBaseWidget.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "DebugHelper.h"
void UMenuBaseWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

   
    SetVisibility(ESlateVisibility::Hidden);
}

void UMenuBaseWidget::ShowMenu(bool bPauseGame, bool bUIOnlyInput)
{
   
    if (!IsInViewport())
    {
        AddToViewport();
    }

    SetVisibility(ESlateVisibility::Visible);

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        
        if (bUIOnlyInput)
        {
            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(TakeWidget());
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            PC->SetInputMode(InputMode);
        }
        else
        {
            FInputModeGameAndUI InputMode;
            InputMode.SetWidgetToFocus(TakeWidget());
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            PC->SetInputMode(InputMode);
        }

        PC->bShowMouseCursor = true;
    }


    if (bPauseGame)
    {
        UGameplayStatics::SetGamePaused(GetWorld(), true);
    }

    OnMenuOpened(); 
}

void UMenuBaseWidget::HideMenu(bool bUnpauseGame)
{
    SetVisibility(ESlateVisibility::Hidden);

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        PC->SetInputMode(FInputModeGameOnly());
        PC->bShowMouseCursor = false;
    }

    if (bUnpauseGame)
    {
        UGameplayStatics::SetGamePaused(GetWorld(), false);
    }

    OnMenuClosed(); 
}

void UMenuBaseWidget::OnMenuOpened()
{
    Debug::Print("GameisPaused");
    
}

void UMenuBaseWidget::OnMenuClosed()
{
   
    Debug::Print("GameisUnPaused");
}
