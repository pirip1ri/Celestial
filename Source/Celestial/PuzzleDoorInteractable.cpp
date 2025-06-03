// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleDoorInteractable.h"
#include "Components/BoxComponent.h"
#include "DebugHelper.h"
APuzzleDoorInteractable::APuzzleDoorInteractable()
{
   
    DoorTriggerCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorTriggerCollider"));
    DoorTriggerCollider->SetupAttachment(RootComponent);

    DoorTrigger = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorTrigger"));
    DoorTrigger->SetupAttachment(DoorTriggerCollider);
    DoorTrigger->SetupAttachment(MeshComponent);
}

void APuzzleDoorInteractable::HitTarget()
{
    
    if (HitMaterial)
    {
        DoorTrigger->SetMaterial(0, HitMaterial);
    }
    bIsTriggerByPuzzle = false;

    // Open the door using the provided logic
    SetDoorState(EDoorState::Open);
	//Debug::Print("Door is now open");	
}

void APuzzleDoorInteractable::InteractAbility_Implementation()
{
	Super::InteractAbility_Implementation();
}
