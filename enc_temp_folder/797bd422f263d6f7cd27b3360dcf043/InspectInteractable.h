// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "InspectInteractable.generated.h"

/**
 * 
 */
class UMenuBaseWidget;
UCLASS()
class CELESTIAL_API AInspectInteractable : public AInteractable
{
	GENERATED_BODY()
public:
	AInspectInteractable();
	virtual void InteractAbility_Implementation()override;

	void EndInspection();
	UPROPERTY(EditDefaultsOnly)
	USceneComponent* InspectCameraAnchor;
private:
	UPROPERTY()
	UMenuBaseWidget* Widget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMenuBaseWidget>WidgetClass;
};
