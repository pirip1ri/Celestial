// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Collectable.h"
#include "KeyPickup.generated.h"

/**
 * 
 */
class UInteractionPromptWidget;
UCLASS()
class CELESTIAL_API AKeyPickup : public ACollectable
{
	GENERATED_BODY()
public:
	AKeyPickup();
protected:

	virtual void CollectItem() override;

private:
	// Incase there are multiple keys//
	UPROPERTY(EditAnywhere, Category = "Key")
	FName KeyID;

	UPROPERTY(EditAnywhere)
	FText KeyState;



	UPROPERTY(EditAnywhere)
	TSubclassOf<UInteractionPromptWidget>WidgetCompClass;
};
