// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "InspectInteractables.generated.h"

/**
 * 
 */
class APlayerCharacter;
class PauseMenuWidget;
class UMenuBaseWidget;
UCLASS()
class CELESTIAL_API AInspectInteractables : public AInteractable
{
	GENERATED_BODY()
public:
	AInspectInteractables();

	void InteractAbility();
	
	virtual void Tick(float DeltaTime) override;

	void EndInspection();
	APlayerCharacter* GetTargetPlayer();

	// Interface implementation
	//virtual void OnInteract(APlayerCharacter* PlayerCharacter) override;

	UPROPERTY()
	APlayerCharacter* TargetPlayer;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* InspectCameraAnchor;

	UPROPERTY(EditDefaultsOnly, Category = "Inspect")
	float InspectArmLength = 150.f;

	bool bIsInspecting = false;

private:
	UPROPERTY()
	UMenuBaseWidget* Widget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMenuBaseWidget> WidgetClass;

	FTimerHandle ContextualMenuDelayHandle;
	bool bIsExitingInspection = false;
};



	

