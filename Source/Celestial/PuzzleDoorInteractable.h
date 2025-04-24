// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableDoor.h"
#include "PuzzleDoorInteractable.generated.h"

/**
 * 
 */
class ARotatereflecter;
class UBoxComponent;
UCLASS()
class CELESTIAL_API APuzzleDoorInteractable : public AInteractableDoor
{
	GENERATED_BODY()

public:
	APuzzleDoorInteractable();

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* DoorTrigger;
	
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* DoorTriggerCollider;

	UPROPERTY()
	ARotatereflecter* Beam;

	UPROPERTY(EditAnywhere, Category = "Puzzle")
	UMaterialInterface* HitMaterial;

	void HitTarget();
private:
	virtual void InteractAbility_Implementation()override;
	
};
