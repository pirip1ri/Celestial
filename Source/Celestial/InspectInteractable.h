// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "InspectInteractable.generated.h"

/**
 * 
 */
class APlayerCharacter;
class PauseMenuWidget;
class UMenuBaseWidget;
UCLASS()
class CELESTIAL_API AInspectInteractable : public AInteractable
{
	GENERATED_BODY()
public:
	AInspectInteractable();
	virtual void InteractAbility_Implementation()override;

virtual	void Tick(float DeltaTime)override;

	UPROPERTY()
	APlayerCharacter* TargetPlayer;

	bool bIsInspecting = false;
	void EndInspection();
	UPROPERTY(EditDefaultsOnly)
	USceneComponent* InspectCameraAnchor;
private:
	UPROPERTY()
	UMenuBaseWidget* Widget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMenuBaseWidget>WidgetClass;
	FTimerHandle ContextualMenuDelayHandle;
	bool bIsExitingInspection = false;

	UPROPERTY(EditDefaultsOnly, Category = "Inspect")
	float InspectArmLength = 150.f; // or 100.f or whatever looks good

};
