// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include <EnhancedInputSubsystemInterface.h>
#include "InteractableInterface.h"  
#include "Rotatereflecter.generated.h"

/**
 * 
 */
UCLASS()
class CELESTIAL_API ARotatereflecter : public AInteractable, public IInteractableInterface
{
	GENERATED_BODY()
public:
	ARotatereflecter();
	void BeginPlay();
	UStaticMeshComponent* GetMesh() { return MeshComponent; }
	void InteractWithReflector();
	void RotateReflector(FString Direction);

	virtual void OnInteract(APlayerCharacter* PlayerCharacter) override;
virtual	void Tick(float DeltaTime) override;

	float RotationSpeed = 0.5f; 
	bool bShouldRotate;
virtual	void ReflectBeam(FVector HitPoint, FVector IncomingDirection, int32 RemainingBounces);
	void AddYawInput(float DeltaYaw);
	void AddPitchInput(float DeltaPitch);
	//virtual void InteractAbility_Implementation()override;
	//void ReflectBeam(FVector HitPoint, FVector IncomingDirection);
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MirrorMesh;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MirrorStandMesh;
	
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Reflectiveball;
	
	UPROPERTY(EditAnywhere)
	USceneComponent* Root;
	bool bIsInteracting;
	FRotator TargetRotation;



};
