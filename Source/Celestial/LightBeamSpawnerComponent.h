// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LightBeamSpawnerComponent.generated.h"

class ALightBeamActor;
class USceneComponent;
class ACharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CELESTIAL_API ULightBeamSpawnerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    ULightBeamSpawnerComponent();

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    void SpawnBeam(FVector Start, FVector Direction);

    UFUNCTION(BlueprintCallable)
    void DestroyBeam();

    // Name of the socket on the mesh where the beam should originate
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Beam")
    FName LightBeamSocketName = "ChestSocket";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Beam")
    TSubclassOf<ALightBeamActor> LightBeamActorClass;

private:
    UPROPERTY()
    ALightBeamActor* ActiveBeam = nullptr;
};
