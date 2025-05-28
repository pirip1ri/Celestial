// Fill out your copyright notice in the Description page of Project Settings.

#include "CelestialTutorialPromptManager.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
ACelestialTutorialPromptManager::ACelestialTutorialPromptManager()
{
	PrimaryActorTick.bCanEverTick = false;
	CurrentPromptIndex = 0;
	CurrentWidget = nullptr;
}

void ACelestialTutorialPromptManager::BeginPlay()
{
	Super::BeginPlay();

	if (TutorialWidgetClass)
	{
		ShowNextPrompt();
	}
}

void ACelestialTutorialPromptManager::ShowNextPrompt()
{
	// If we've reached the end of the prompts, hide the widget and return
	if (!Prompts.IsValidIndex(CurrentPromptIndex))
	{
		if (CurrentWidget)
		{
			CurrentWidget->RemoveFromParent();
			CurrentWidget = nullptr;
		}
		return;
	}

	const FPromptData& Prompt = Prompts[CurrentPromptIndex];

	// Clean up old widget
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}

	// Create new widget
	CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), TutorialWidgetClass);
	if (CurrentWidget)
	{
		CurrentWidget->AddToViewport();

		if (UTextBlock* TextBlock = Cast<UTextBlock>(CurrentWidget->GetWidgetFromName(TEXT("PromptText"))))
		{
			TextBlock->SetText(Prompt.PromptText);
		}
	}
}


void ACelestialTutorialPromptManager::AdvancePromptFromController()
{
	if (Prompts.IsValidIndex(CurrentPromptIndex))
	{
		Prompts[CurrentPromptIndex].bCompleted = true;

		// Start color interpolation
		ColorBlendTimeElapsed = 0.f;
		GetWorld()->GetTimerManager().SetTimer(ColorBlendTimerHandle, this, &ACelestialTutorialPromptManager::UpdateTextColor, 0.01f, true);
	}
}

void ACelestialTutorialPromptManager::UpdateTextColor()
{
	if (!CurrentWidget) return;

	if (UTextBlock* TextBlock = Cast<UTextBlock>(CurrentWidget->GetWidgetFromName(TEXT("PromptText"))))
	{
		ColorBlendTimeElapsed += GetWorld()->GetDeltaSeconds();
		float Alpha = FMath::Clamp(ColorBlendTimeElapsed / ColorBlendDuration, 0.f, 1.f);
		FLinearColor BlendedColor = FLinearColor::LerpUsingHSV(StartColor, TargetColor, Alpha);

		TextBlock->SetColorAndOpacity(FSlateColor(BlendedColor));

		if (Alpha >= 1.f)
		{
			GetWorld()->GetTimerManager().ClearTimer(ColorBlendTimerHandle);

			// After color is fully green, wait briefly then advance
			FTimerHandle DelayHandle;
			GetWorld()->GetTimerManager().SetTimer(DelayHandle, this, &ACelestialTutorialPromptManager::AdvanceToNextPrompt, 0.5f, false);
		}
	}
}


void ACelestialTutorialPromptManager::AdvanceToNextPrompt()
{
	CurrentPromptIndex++;
	ShowNextPrompt();
}

void ACelestialTutorialPromptManager::HandlePromptInput(const FInputActionInstance& ActionInstance)
{
	if (!Prompts.IsValidIndex(CurrentPromptIndex)) return;

	const FPromptData& CurrentPrompt = Prompts[CurrentPromptIndex];
	if (ActionInstance.GetSourceAction() == CurrentPrompt.InputAction)
	{
		AdvancePromptFromController();
	}
}
