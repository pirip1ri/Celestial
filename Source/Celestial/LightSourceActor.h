// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightSourceActor.generated.h"

class UArrowComponent;
UCLASS()
class CELESTIAL_API ALightSourceActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ALightSourceActor();


    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere)
    float BeamLength = 1000.0f;

    UPROPERTY(EditAnywhere)
    USceneComponent* Root;

    UPROPERTY(EditAnywhere)
    UArrowComponent* DirectionArrow;

    void FireBeam();

};
