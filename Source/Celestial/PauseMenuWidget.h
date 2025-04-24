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
class UImage;
UCLASS()
class CELESTIAL_API UPauseMenuWidget : public UMenuBaseWidget
{
	GENERATED_BODY()

public:
    void TogglePauseMenu();
    void ToggleOtherMenu();

    virtual void OnMenuOpened() override;
    virtual void OnMenuClosed() override;
    void ToggleContextualMenu(bool bShow, const FText& Message, UTexture2D* OptionalImage, bool bShowPauseButtons);
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

    UPROPERTY(meta = (BindWidget))
    UImage* InspectImage;
  

    UFUNCTION()
    void OnResumeClicked();
    UFUNCTION()
    void OnMainMenuClicked();
    UFUNCTION()
    void OnQuitClicked();
};
