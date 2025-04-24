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
	if (!Prompts.IsValidIndex(CurrentPromptIndex)) return;

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
		CurrentPromptIndex++;
		ShowNextPrompt();
	}
}
