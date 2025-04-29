// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include <EnhancedInputSubsystemInterface.h>
#include "Rotatereflecter.generated.h"

/**
 * 
 */
UCLASS()
class CELESTIAL_API ARotatereflecter : public AInteractable
{
	GENERATED_BODY()
public:
	ARotatereflecter();
	void BeginPlay();
	UStaticMeshComponent* GetMesh() { return MeshComponent; }
	void InteractWithReflector();
	void RotateReflector(FString Direction);

virtual	void Tick(float DeltaTime) override;

	float RotationSpeed = 0.5f; 
	bool bShouldRotate = false;
virtual	void ReflectBeam(FVector HitPoint, FVector IncomingDirection, int32 RemainingBounces);
	void AddYawInput(float DeltaYaw);
	void AddPitchInput(float DeltaPitch);
	virtual void InteractAbility_Implementation()override;
	//void ReflectBeam(FVector HitPoint, FVector IncomingDirection);
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MirrorMesh;
	
	UPROPERTY(EditAnywhere)
	USceneComponent* Root;
	bool bIsInteracting;
	FRotator TargetRotation;


	//bool IsInteracting() const;

	//void StartInteraction();

	//void EndInteraction();

};
