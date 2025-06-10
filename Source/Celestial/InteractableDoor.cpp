// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableDoor.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "DebugHelper.h"

AInteractableDoor::AInteractableDoor()
{

	ClosedRotation = FRotator::ZeroRotator;
	OpenRotation = FRotator(0.f, -90.f, 0.f);
}

void AInteractableDoor::SetDoorState(EDoorState NewState)
{
	if (DoorState != NewState)
	{
		DoorState = NewState;
		OnDoorStateChanged.Broadcast(DoorState);
	}

	
	if (NewState == EDoorState::Open)
	{
		MeshComponent->SetRelativeRotation(OpenRotation);
	}
	else
	{
		MeshComponent->SetRelativeRotation(ClosedRotation);
	}
}

void AInteractableDoor::InteractAbility_Implementation()
{
	Super::InteractAbility_Implementation();

	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!Player)
		return;

	if (DoorState == EDoorState::Open)
		return;

	if (Player->HasKey(RequiredKeyID))
	{
		SetDoorState(EDoorState::Open);
		Debug::Print(FString::Printf(TEXT("Door Unlocked: %s"), *RequiredKeyID.ToString()));
	}
	else
	{
		Debug::Print(FString::Printf(TEXT("Player missing key: %s"), *RequiredKeyID.ToString()));
	}
}
