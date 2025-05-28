// Fill out your copyright notice in the Description page of Project Settings.


#include "dialsInteractable.h"




AdialsInteractable::AdialsInteractable()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Dial1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Dial1"));
	Dial1->SetupAttachment(RootComponent);
	Dial1->SetRelativeLocation(FVector(0.f, -150.f, 0.f));
	SymbolMeshes.Add(Dial1);

	Dial2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Dial2"));
	Dial2->SetupAttachment(RootComponent);
	Dial2->SetRelativeLocation(FVector(0.f, -50.f, 0.f));
	SymbolMeshes.Add(Dial2);

	Dial3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Dial3"));
	Dial3->SetupAttachment(RootComponent);
	Dial3->SetRelativeLocation(FVector(0.f, 50.f, 0.f));
	SymbolMeshes.Add(Dial3);

	Dial4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Dial4"));
	Dial4->SetupAttachment(RootComponent);
	Dial4->SetRelativeLocation(FVector(0.f, 150.f, 0.f));
	SymbolMeshes.Add(Dial4);
}
void AdialsInteractable::InteractAbility_Implementation()
{
	Super::InteractAbility_Implementation();

	// Only rotate one dial at a time
	if (SymbolMeshes.IsValidIndex(CurrentDialIndex))
	{
		UStaticMeshComponent* Dial = SymbolMeshes[CurrentDialIndex];

		// Rotate it like a barrel (roll = x-axis)
		FRotator NewRotation = Dial->GetRelativeRotation();
		NewRotation.Yaw+= 20.0f;
		Dial->SetWorldRotation(NewRotation);

		UE_LOG(LogTemp, Warning, TEXT("Rotated dial %d"), CurrentDialIndex);

		// Cycle to the next dial
		CurrentDialIndex = (CurrentDialIndex + 1) % SymbolMeshes.Num();
	}
}
