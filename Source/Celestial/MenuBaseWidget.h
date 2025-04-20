// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MenuBaseWidget.generated.h"

/**
 * 
 */
UCLASS()
class CELESTIAL_API UMenuBaseWidget : public UUserWidget
{
	GENERATED_BODY()
public:

    virtual void ShowMenu(bool bPauseGame = false, bool bUIOnlyInput = true);

  
    virtual void HideMenu(bool bUnpauseGame = false);

protected:
    virtual void NativeOnInitialized() override;
    virtual void OnMenuOpened();  
    virtual void OnMenuClosed();
};

