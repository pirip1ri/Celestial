// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundBase.h" 
#include "Collectable.generated.h"

class ATriggerBox;

UCLASS()
class CELESTIAL_API ACollectable : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACollectable();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void CheckOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	//---- Jade "made this virual"---//
	UFUNCTION()
	virtual	void CollectItem();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystemComponent* CollectEffect;

	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* CollectSound;

	FVector InitialLocation;
	float RunningTime;

	UPROPERTY(EditAnywhere, Category = "Collectable|Animation")
	float BobbingAmplitude = 20.0f; // how far it bobs

	UPROPERTY(EditAnywhere, Category = "Collectable|Animation")
	float BobbingSpeed = 2.0f; // how fast it bobs

	UPROPERTY(EditAnywhere, Category = "Collectable|Animation")
	float RotationRate = 50.0f;
};
