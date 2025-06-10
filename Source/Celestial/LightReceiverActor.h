// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightReceiverActor.generated.h"

UCLASS()
class CELESTIAL_API ALightReceiverActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ALightReceiverActor();

	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	void ActivateReceiver();

};
