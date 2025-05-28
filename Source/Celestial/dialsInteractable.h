// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "dialsInteractable.generated.h"

/**
 * 
 */
UCLASS()
class CELESTIAL_API AdialsInteractable : public AInteractable
{
	GENERATED_BODY()


	virtual void InteractAbility_Implementation()override;
public:
	AdialsInteractable();
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Dial1;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Dial2;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Dial3;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Dial4;

	UPROPERTY(VisibleAnywhere)
	TArray<UStaticMeshComponent*> SymbolMeshes;
private:
	

	UPROPERTY(EditAnywhere)
	int32 CurrentDialIndex = 0;
};
