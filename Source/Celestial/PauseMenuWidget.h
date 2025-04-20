// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuBaseWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "PauseMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class CELESTIAL_API UPauseMenuWidget : public UMenuBaseWidget
{
	GENERATED_BODY()

public:
    void TogglePauseMenu();

    virtual void OnMenuOpened() override;
    virtual void OnMenuClosed() override;
protected:
    virtual void NativeOnInitialized() override;
 
private:
    bool bIsVisible = false;
    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* RootBox;

    UPROPERTY(meta = (BindWidget))
    class UButton* ResumeButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* MainMenuButton;
    
    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text;

  

    UFUNCTION()
    void OnResumeClicked();
    UFUNCTION()
    void OnMainMenuClicked();
    UFUNCTION()
    void OnQuitClicked();
};
