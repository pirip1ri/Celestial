// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"

// Sets default values
AInteractable::AInteractable()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractable::InteractAbility_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Base interactable interaction"));
}

