// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "CelestialTutorialPromptManager.generated.h"



USTRUCT(BlueprintType)
struct FPromptData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText PromptText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UInputAction> InputAction;

	UPROPERTY(BlueprintReadOnly)
	bool bCompleted = false;
};


class UInputMappingContext;
UCLASS()
class CELESTIAL_API ACelestialTutorialPromptManager : public AActor
{
	GENERATED_BODY()

protected:
	ACelestialTutorialPromptManager();
	virtual void BeginPlay() override;
	void ShowNextPrompt();

	UPROPERTY(EditAnywhere, Category = "Tutorial")
	TSubclassOf<UUserWidget> TutorialWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Tutorial")
	TArray<FPromptData> Prompts;

	UPROPERTY()
	UUserWidget* CurrentWidget;


public:
	UFUNCTION()
	void AdvancePromptFromController();
	UFUNCTION(BlueprintCallable)
	const TArray<FPromptData>& GetPromptList() const { return Prompts; }
	int32 CurrentPromptIndex;

	/*UFUNCTION()
	void HandleCurrentInput(const struct FInputActionInstance& ActionInstance);*/
};
